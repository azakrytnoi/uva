#pragma once

#include <fstream>
#include <mutex>
#include <memory>
#include <utility>
#include <sstream>
#include <iosfwd>

class logger {
private:
    static logger* instance_;

    static void check();

    logger();

    std::ofstream out_;
    static std::mutex log_mutex_;

    class log_stream : public std::ostream {
        std::stringstream buff_;
        logger& owner_;

        typedef std::basic_ios<char, std::char_traits<char>> _Myios;
    public:
        log_stream(logger& owner) : buff_(), owner_(owner), std::ostream(owner_.out_.rdbuf()) {}
        virtual ~log_stream()
        {
            std::lock_guard<std::mutex> l(owner_.log_mutex_);
            (*(std::ostream*)this) << buff_.rdbuf() << std::endl;
            owner_.out_.flush();
        }

        log_stream& operator << (bool val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (char val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (const char* val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (unsigned char val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (short val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (unsigned short val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (int val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (unsigned int val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (long val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (unsigned long val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (long long val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (unsigned long long val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (float val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (double val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (long double val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (const void* val)
        {
            buff_ << val;
            return *this;
        }
        log_stream& operator << (std::streambuf* val)
        {
            buff_ << val;
            return *this;
        }

        log_stream&  operator<<(log_stream & (__cdecl* _Pfn)(log_stream&))
        {
            // call basic_ostream manipulator
            return ((*_Pfn)(*this));
        }

        log_stream&  operator<<(_Myios & (__cdecl* _Pfn)(_Myios&))
        {
            // call basic_ios manipulator
            (*_Pfn)(*(_Myios*)this);
            return (*this);
        }

        log_stream&  operator<<(ios_base & (__cdecl* _Pfn)(ios_base&))
        {
            // call ios_base manipulator
            (*_Pfn)(*(ios_base*)this);
            return (*this);
        }
    };

    log_stream* sstream_;

public:
    static log_stream& log()
    {
        check();
        return *instance_->sstream_;
    }
};
