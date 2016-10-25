#pragma once

class U10128
{
public:
	static const char * libname()
	{
		return "u10128";
	}
    U10128() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10128> pu10128;
#endif
