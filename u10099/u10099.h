#pragma once

class U10099
{
public:
	static const char * libname()
	{
		return "u10099";
	}
    U10099() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10099> pu10099;
#endif
