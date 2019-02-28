#pragma once

class U436
{
public:
	static const char * libname()
	{
		return "u436";
	}
    U436() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U436> pu436;
#endif
