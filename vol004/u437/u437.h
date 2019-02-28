#pragma once

class U437
{
public:
	static const char * libname()
	{
		return "u437";
	}
    U437() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U437> pu437;
#endif
