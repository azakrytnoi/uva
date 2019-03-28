#pragma once

class U10009
{
public:
	static const char * libname()
	{
		return "u10009";
	}
    U10009() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10009> pu10009;
#endif
