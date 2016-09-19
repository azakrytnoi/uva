#pragma once

class U10035
{
public:
	static const char * libname()
	{
		return "u10035";
	}
    U10035() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U10035> pu10035;
#endif
