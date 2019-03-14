#pragma once

class U10005
{
public:
	static const char * libname()
	{
		return "u10005";
	}
    U10005() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10005> pu10005;
#endif
