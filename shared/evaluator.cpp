#include <iostream>


#include <dlfcn.h>
#define __cdecl

#include "evaluator.h"

#include <map>
#include <typeinfo>
#include <memory>
#include <algorithm>
#include <exception>
#include <charconv>

#if 0
    #include <dirent.h>
#else
    #include <experimental/filesystem>
#endif

namespace {
    std::map<std::string, std::shared_ptr<uva_wraper>> g_cache;

    template<class... Ts> struct populate {};

    template<class Tp, class... Ts>
    struct populate<Tp, Ts...> : populate<Ts...>
    {
        populate() : populate<Ts...>()
        {
            std::string tp_name(typeid(Tp).name());
            tp_name = tp_name.substr(tp_name.find('U') + 1);
            g_cache[tp_name] = std::make_shared<evaluator<Tp>>("../u" + tp_name + "/u" + tp_name + ".txt");
        }
    };

    void populate_uva()
    {
#if 0
        DIR* dp;

        if ((dp = opendir("../lib")) != nullptr)
        {
            dirent* dref;

            while ((dref = readdir(dp)) != nullptr)
            {
                if (dref->d_name[0] != '.')
                {
                    std::string libname(dref->d_name);
                    std::string p_name (libname.substr(4, libname.find('.') - 4));
                    std::stringstream vol_in(p_name), vol_out;
                    size_t num(0);
                    vol_in >> num;
                    vol_out << std::setw(3) << std::setfill('0') << std::right << (num / 100);
                    g_cache[p_name] = std::make_shared<dyn_evaluator>("../vol" + vol_out.str() + "/u" + p_name + "/u" + p_name + ".txt", "u" + p_name);
                }
            }
        }

#else
        namespace fs = std::experimental::filesystem;

        for (auto& p : fs::directory_iterator("../lib"))
        {
            std::string libname(fs::path(p).filename());

            if (libname.substr(0, 3) == "lib")
            {
                auto p_name (libname.substr(3, libname.find('.') - 3));
                std::stringstream vol_in(p_name), vol_out;
                size_t num(0);
                char tmp(0);
                vol_in >> tmp >> num;
                vol_out << std::setw(3) << std::setfill('0') << std::right << (num / 100);
                g_cache[p_name] = std::make_shared<dyn_evaluator>("../vol" + vol_out.str() + "/" + p_name + "/" + p_name + ".txt", p_name);
            }
        }

#endif
    }
}

//#include "uvas.h"

int main(int argc, char** argv)
{
    populate_uva();

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            std::cout << argv[i] << std::endl;
            auto uva = g_cache.find(argv[i]);

            if (uva != g_cache.end())
            {
                (*(uva->second))();
            }
        }
    }
    else
    {
        std::for_each(g_cache.begin(), g_cache.end(), [](auto uva)
        {
            (*(uva.second))();
        });
    }
}

uva_wraper::invoker uva_wraper::prepare(const std::string& baseName)
{
    handle_ = dlopen(("lib" + baseName + ".so").c_str(), RTLD_LAZY);

    if (!handle_)
    {
        std::cout << "failure loading library" << std::endl;
        throw std::exception();
    }

    invoker fnc = (invoker)dlsym(handle_, "invoke");

    if (!fnc)
    {
        std::cout << "failure locate function" << std::endl;
        throw std::exception();
    }

    return fnc;
}

void uva_wraper::release()
{
    if (handle_)
    {
        dlclose(handle_);
        handle_ = nullptr;
    }
}

void dyn_evaluator::operator()()
{
    std::string tp_name(libname_);
    tp_name = tp_name.substr(tp_name.find('u'));
    std::cout << tp_name << ": << " << source_ << std::endl << std::string(40, '-') << std::endl;
    std::ifstream in(source_.c_str());
    uint64_t elapsed(0);
    {
        invoker fnc = prepare(libname_);
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
    std::cout << std::endl << std::string(40, '-') << std::endl
              << "Elapsed: " << std::fixed << std::setprecision(4) << (elapsed / 1000000.0) << "ms." << std::endl;
}
