#pragma once

class U495
{
public:
	static const char * libname()
	{
		return "u495";
	}
    U495() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U495> pu495;
#endif
