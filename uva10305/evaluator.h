#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <functional>
#include <iosfwd>
#include <sstream>



template <typename char_type,
          typename traits = std::char_traits<char_type> >
class basic_teebuf:
    public std::basic_streambuf<char_type, traits>
{
public:
    typedef typename traits::int_type int_type;

    basic_teebuf(std::basic_streambuf<char_type, traits> * sb1,
                 std::basic_streambuf<char_type, traits> * sb2)
        : sb1(sb1)
        , sb2(sb2)
    {
    }

private:
    virtual int sync()
    {
        int const r1 = sb1->pubsync();
        int const r2 = sb2->pubsync();
        return r1 == 0 && r2 == 0 ? 0 : -1;
    }

    virtual int_type overflow(int_type c)
    {
        int_type const eof = traits::eof();

        if (traits::eq_int_type(c, eof))
        {
            return traits::not_eof(c);
        }
        else
        {
            char_type const ch = traits::to_char_type(c);
            int_type const r1 = sb1->sputc(ch);
            int_type const r2 = sb2->sputc(ch);

            return
                traits::eq_int_type(r1, eof) ||
                traits::eq_int_type(r2, eof) ? eof : c;
        }
    }

private:
    std::basic_streambuf<char_type, traits> * sb1;
    std::basic_streambuf<char_type, traits> * sb2;
};

typedef basic_teebuf<char> teebuf;



class teestream : public std::ostream
{
public:
    teestream(std::ostream & o1, std::ostream & o2);
private:
    teebuf tbuf;
};

teestream::teestream(std::ostream & o1, std::ostream & o2)
    : std::ostream(&tbuf), tbuf(o1.rdbuf(), o2.rdbuf())
{
}

class wraper
{
public:
	virtual ~wraper() {}
	virtual void operator()() = 0;
};

template<typename Tp>
class evaluator : public wraper
{
public:
	explicit evaluator(const std::string& source) : source_(source)
	{
	}

    virtual ~evaluator()
	{
	}

	virtual void operator ()();

private:
	std::string source_;
	Tp tp_;
};

namespace
{
template <typename T>
class io_wrapper
{
public:
    explicit io_wrapper(T& stream, std::streambuf* streambuf) : stream_(stream)
    {
        streambuf_ = stream_.rdbuf(streambuf);
    }

    ~io_wrapper()
    {
        stream_.rdbuf(streambuf_);
    }

private:
    T& stream_;
    std::streambuf* streambuf_;
};

}

template<typename Tp>
void evaluator<Tp>::operator()()
{
    std::ifstream in(source_.c_str());
    std::stringstream out;
    {
        io_wrapper<std::ostream> wrap_in (std::cout, out.rdbuf());
        io_wrapper<std::istream> wrap_out (std::cin, in.rdbuf());
        tp_();
    }
    std::ofstream log((source_ + ".out").c_str());
    teestream tee(log, std::cout);
    tee << out.str();
}
