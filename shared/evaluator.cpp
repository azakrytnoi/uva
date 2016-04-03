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

namespace
{
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

}

#include "uvas.h"

int main(int argc, char** argv)
{
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

uva_wraper::invoker uva_wraper::prepare(const std::string & baseName)
{
#ifdef _WIN32
    HINSTANCE hGetProcIDDLL = LoadLibraryA((baseName + ".dll").c_str());
    if (!hGetProcIDDLL) {
        std::cout << "failure loading library" << std::endl;
        throw std::exception(baseName.c_str());
    }
    invoker fnc = (invoker)GetProcAddress(hGetProcIDDLL, "invoke");
#else
    void* handle = dlopen(("lib" + baseName + ".so").c_str(), RTLD_LAZY);
    if (!handle) {
        std::cout << "failure loading library" << std::endl;
        throw std::exception();
    }
    invoker fnc = (invoker)dlsym(handle, "invoke");
#endif // _WIN32
    if (!fnc) {
        std::cout << "failure locate function" << std::endl;
        throw std::exception();
    }
    return fnc;
}
