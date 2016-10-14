#pragma once

class U10104
{
public:
	static const char * libname()
	{
		return "u10104";
	}
    U10104() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10104> pu10104;
#endif
