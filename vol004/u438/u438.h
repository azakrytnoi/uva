#pragma once

class U438
{
public:
	static const char * libname()
	{
		return "u438";
	}
    U438() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U438> pu438;
#endif
