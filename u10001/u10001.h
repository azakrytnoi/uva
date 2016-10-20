#pragma once

class U10001
{
public:
	static const char * libname()
	{
		return "u10001";
	}
    U10001() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10001> pu10001;
#endif
