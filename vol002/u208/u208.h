#pragma once

class U208
{
public:
	static const char * libname()
	{
		return "u208";
	}
    U208() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U208> pu208;
#endif
