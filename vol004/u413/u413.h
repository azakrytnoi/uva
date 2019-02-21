#pragma once

class U413
{
public:
	static const char * libname()
	{
		return "u413";
	}
    U413() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U413> pu413;
#endif
