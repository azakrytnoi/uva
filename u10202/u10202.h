#pragma once

class U10202
{
public:
	static const char * libname()
	{
		return "u10202";
	}
    U10202() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U10202> pu10202;
#endif
