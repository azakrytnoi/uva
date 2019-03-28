#pragma once

class U10012
{
public:
	static const char * libname()
	{
		return "u10012";
	}
    U10012() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10012> pu10012;
#endif
