#pragma once

class U10032
{
public:
	static const char * libname()
	{
		return "u10032";
	}
    U10032() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10032> pu10032;
#endif
