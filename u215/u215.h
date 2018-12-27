#pragma once

class U215
{
public:
	static const char * libname()
	{
		return "u215";
	}
    U215() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U215> pu215;
#endif
