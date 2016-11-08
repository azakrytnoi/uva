#pragma once

class U10004
{
public:
	static const char * libname()
	{
		return "u10004";
	}
    U10004() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10004> pu10004;
#endif
