#pragma once

class U10014
{
public:
	static const char * libname()
	{
		return "u10014";
	}
    U10014() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10014> pu10014;
#endif
