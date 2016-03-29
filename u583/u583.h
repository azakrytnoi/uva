#pragma once

class U583
{
public:
	static const char * libname()
	{
		return "u583";
	}
    U583() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U583> pu583;
#endif
