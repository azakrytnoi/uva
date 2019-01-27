#pragma once

class U10368
{
public:
	static const char * libname()
	{
		return "u10368";
	}
    U10368() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10368> pu10368;
#endif
