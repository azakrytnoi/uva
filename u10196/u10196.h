#pragma once

class U10196
{
public:
	static const char * libname()
	{
		return "u10196";
	}
    U10196() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U10196> pu10196;
#endif
