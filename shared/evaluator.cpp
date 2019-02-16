#include <iostream>


#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
    #define __cdecl
#endif

#include "evaluator.h"

#include <map>
#include <typeinfo>
#include <memory>
#include <algorithm>
#include <exception>

#include <dirent.h>

namespace {
    std::map<std::string, std::shared_ptr<uva_wraper>> g_cache;

    template<class... Ts> struct populate {};

    template<class Tp, class... Ts>
    struct populate<Tp, Ts...> : populate<Ts...> {
        populate() : populate<Ts...>()
        {
            std::string tp_name(typeid(Tp).name());
            tp_name = tp_name.substr(tp_name.find('U') + 1);
            g_cache[tp_name] = std::make_shared<evaluator<Tp>>("../u" + tp_name + "/u" + tp_name + ".txt");
        }
    };

    void populate_uva()
    {
        DIR* dp;

        if ((dp = opendir("../lib")) != nullptr) {
            dirent* dref;

            while ((dref = readdir(dp)) != nullptr) {
                if (dref->d_name[0] != '.') {
                    std::string libname(dref->d_name);
                    std::string p_name (libname.substr(4, libname.find('.') - 4));
                    g_cache[p_name] = std::make_shared<dyn_evaluator>("../u" + p_name + "/u" + p_name + ".txt", "u" + p_name);
                }
            }
        }
    }
}

//#include "uvas.h"

int main(int argc, char** argv)
{
    populate_uva();

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            auto uva = g_cache.find(argv[i]);

            if (uva != g_cache.end()) {
                (*(uva->second))();
            }
        }

    } else {
        std::for_each(g_cache.begin(), g_cache.end(), [](auto uva) {
            (*(uva.second))();
        });
    }
}

uva_wraper::invoker uva_wraper::prepare(const std::string& baseName)
{
#ifdef _WIN32
    hGetProcIDDLL_ = LoadLibraryA((baseName + ".dll").c_str());

    if (!hGetProcIDDLL_) {
        std::cout << "failure loading library" << std::endl;
        throw std::exception(baseName.c_str());
    }

    invoker fnc = (invoker)GetProcAddress(hGetProcIDDLL_, "invoke");
#else
    handle_ = dlopen(("lib" + baseName + ".so").c_str(), RTLD_LAZY);

    if (!handle_) {
        std::cout << "failure loading library" << std::endl;
        throw std::exception();
    }

    invoker fnc = (invoker)dlsym(handle_, "invoke");
#endif // _WIN32

    if (!fnc) {
        std::cout << "failure locate function" << std::endl;
        throw std::exception();
    }

    return fnc;
}

void uva_wraper::release()
{
#ifdef _WIN32

    if (hGetProcIDDLL_) {
        FreeLibrary(hGetProcIDDLL_);
        hGetProcIDDLL_ = nullptr;
    }

#else

    if (handle_) {
        dlclose(handle_);
        handle_ = nullptr;
    }

#endif // _WIN32
}

void dyn_evaluator::operator()()
{
    std::string tp_name(libname_);
    tp_name = tp_name.substr(tp_name.find('u'));
    std::cout << tp_name << ": << " << source_ << std::endl;
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
    std::cout << std::endl << "Elapsed: " << std::fixed << std::setprecision(4) << (elapsed / 1000000.0) << "ms." << std::endl;
}
