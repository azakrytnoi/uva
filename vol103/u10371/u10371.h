#pragma once

class U10371
{
public:
	static const char * libname()
	{
		return "u10371";
	}
    U10371() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10371> pu10371;
#endif
