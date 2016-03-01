#include <iostream>

#include "evaluator.h"

#include "uvas.h"

#include <map>
#include <typeinfo>
#include <memory>
#include <algorithm>

#include <windows.h>

namespace
{
std::map<std::string, std::shared_ptr<wraper>> g_cache;

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

populate <U100> p;
//populate < U100, U10194, U10219, U10226, U10258, U10301, U10305, U10810, U11136, U11239, U11308, U11340, U11388, U11462, U11495, U127, U11150, U116 > p1;
//populate < U167, U146, U200, U482, U544, U558, U594, U673, U679, U714, U727, U908, U957, U628, U11854, U156, U11636, U10220, U305, U10013, U11461 > p2;
//populate < U10066, U10931, U11185, U900, U10062, U476, U477, U478, U454, U412, U400, U401, U408, U417, U403, U713 > p3;
}

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
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

typedef void (__cdecl *f_instance)();

template<typename Tp>
void wraper::invoke(const std::string & baseName)
{
	HINSTANCE hGetProcIDDLL = LoadLibraryA((baseName + ".dll").c_str());
	if (!hGetProcIDDLL) {
		std::cout << "failure loading library" << std::endl;
		throw std::exception(baseName.c_str());
	}
	f_instance finstance = (f_instance)GetProcAddress(hGetProcIDDLL, "invoke");
	if (!finstance) {
		std::cout << "cuouldn't locate function" << std::endl;
		throw std::exception(baseName.c_str());
	}
	finstance();
}
