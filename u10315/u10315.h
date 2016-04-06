#pragma once

class U10315
{
public:
	static const char * libname()
	{
		return "u10315";
	}
    U10315() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U10315> pu10315;
#endif
