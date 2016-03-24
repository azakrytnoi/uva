#pragma once

class U357
{
public:
	static const char * libname()
	{
		return "u357";
	}
    U357() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U357> pu357;
#endif
