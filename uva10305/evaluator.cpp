#include <iostream>

#include "evaluator.h"

#include "uvas.h"

#include <map>
#include <typeinfo>
#include <memory>

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
			g_cache[tp_name] = std::make_shared<evaluator<Tp>>(tp_name + ".txt");
		}
	};
}

int main(int argc, int** argv)
{
	populate < U100, U10194, U10219, U10226, U10258, U10301, U10305, U10810, U11136, U11239, U11308, U11340, U11388, U11462, U11495, U127,
		U146, U200, U482, U544, U558, U594, U673, U679, U714, U727, U908, U957	> pop;
	if (argc > 1)
	{

	}
}
