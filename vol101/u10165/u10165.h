#pragma once

class U10165
{
public:
	static const char * libname()
	{
		return "u10165";
	}
    U10165() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10165> pu10165;
#endif
