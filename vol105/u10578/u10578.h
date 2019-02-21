#pragma once

class U10578
{
public:
	static const char * libname()
	{
		return "u10578";
	}
    U10578() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10578> pu10578;
#endif
