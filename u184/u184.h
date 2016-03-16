#pragma once

class U184
{
public:
	static const char * libname()
	{
		return "u184";
	}
    U184() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U184> pu184;
#endif
