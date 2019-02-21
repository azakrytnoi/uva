#pragma once

class U739
{
public:
	static const char * libname()
	{
		return "u739";
	}
    U739() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U739> pu739;
#endif
