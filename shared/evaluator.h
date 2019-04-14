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

#include "teestream.h"

#ifndef _WIN32
    #define __cdecl
#endif

class uva_wraper {
public:
    virtual ~uva_wraper() {}
    virtual void operator()() = 0;

protected:
#ifdef _WIN32
    uva_wraper() : hGetProcIDDLL_() {}
#else
    uva_wraper() : handle_() {}
#endif

    uva_wraper(const uva_wraper& rhs) = delete;
    uva_wraper& operator = (const uva_wraper& rhs) = delete;

    typedef void(__cdecl* invoker)();

    invoker prepare(const std::string& baseName);
    void release();

private:
#ifdef _WIN32
    HINSTANCE hGetProcIDDLL_;
#else
    void* handle_;
#endif
};

template<typename Tp>
class evaluator : public uva_wraper {
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

class dyn_evaluator : public uva_wraper {
public:
    explicit dyn_evaluator(const std::string& source, const std::string& libname) : source_(source), libname_(libname)
    {
    }

    virtual ~dyn_evaluator()
    {
    }

    virtual void operator ()();

    dyn_evaluator(const dyn_evaluator& rhs) = delete;
    dyn_evaluator& operator = (const dyn_evaluator& rhs) = delete;

private:
    std::string source_;
    std::string libname_;
};

namespace {
    template <typename T>
    class io_wrapper {
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
        release();
    }
    std::cout << std::endl << "Elapsed: " << std::fixed << std::setprecision(4) << (elapsed / 1000000.0) << "ms." << std::endl;
}
