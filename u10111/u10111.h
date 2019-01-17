#pragma once

class U10111
{
public:
	static const char * libname()
	{
		return "u10111";
	}
    U10111() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10111> pu10111;
#endif
