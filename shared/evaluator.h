#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <functional>
#include <iosfwd>
#include <sstream>
#include <typeinfo>
#include <chrono>
#include <memory>

#ifndef _WIN32
#define __cdecl
#endif

template <typename char_type,
          typename traits = std::char_traits<char_type> >
class basic_teebuf :
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

    virtual ~basic_teebuf() {}

    basic_teebuf(const basic_teebuf& rhs) = delete;
    basic_teebuf& operator = (const basic_teebuf& rhs) = delete;

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

        if (traits::eq_int_type(c, eof)) {
            return traits::not_eof(c);
        } else {
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

class uva_wraper
{
public:
    virtual ~uva_wraper() {}
    virtual void operator()() = 0;

protected:
    typedef void(__cdecl *invoker)();

    invoker prepare(const std::string& baseName);
};

template<typename Tp>
class evaluator : public uva_wraper
{
public:
    explicit evaluator(const std::string& source) : source_(source)
    {
    }

    virtual ~evaluator()
    {
    }

    virtual void operator ()();

    evaluator(const evaluator& rhs) = delete;
    evaluator& operator = (const evaluator& rhs) = delete;

private:
    std::string source_;
};

namespace
{
template <typename T>
class io_wrapper
{
public:
    explicit io_wrapper(T& stream, std::streambuf* streambuf) : stream_(stream), streambuf_(stream_.rdbuf(streambuf))
    {
    }

    ~io_wrapper()
    {
        stream_.rdbuf(streambuf_);
    }

    io_wrapper(const io_wrapper& rhs) = delete;
    io_wrapper& operator = (const io_wrapper& rhs) = delete;

private:
    T& stream_;
    std::streambuf* streambuf_;
};
}

template<typename Tp>
void evaluator<Tp>::operator()()
{
	std::string tp_name(typeid(Tp).name());
	tp_name = tp_name.substr(tp_name.find('U'));
	std::cout << tp_name << ": << " << source_ << std::endl;
	std::ifstream in(source_.c_str());
	uint64_t elapsed(0);
	{
		invoker fnc = prepare(Tp::libname());
		std::ofstream log((source_ + ".out").c_str());
		teestream tee(log, std::cout);
		io_wrapper<std::ostream> wrap_out(std::cout, tee.rdbuf());
		io_wrapper<std::istream> wrap_in(std::cin, in.rdbuf());
		{
			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
			fnc();
			elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count();
		}
	}
	std::cout << std::endl << "Elapsed: " << std::fixed << std::setprecision(4) << (elapsed / 1000000.0) << "ms." << std::endl;
}
