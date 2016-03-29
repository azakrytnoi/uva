#pragma once

class U623
{
public:
	static const char * libname()
	{
		return "u623";
	}
    U623() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U623> pu623;
#endif
