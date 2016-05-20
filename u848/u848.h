#pragma once

class U848
{
public:
	static const char * libname()
	{
		return "u848";
	}
    U848() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U848> pu848;
#endif
