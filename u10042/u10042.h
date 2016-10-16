#pragma once

class U10042
{
public:
	static const char * libname()
	{
		return "u10042";
	}
    U10042() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10042> pu10042;
#endif
