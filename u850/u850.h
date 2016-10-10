#pragma once

class U850
{
public:
	static const char * libname()
	{
		return "u850";
	}
    U850() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U850> pu850;
#endif
