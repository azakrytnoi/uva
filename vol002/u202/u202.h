#pragma once

class U202
{
public:
	static const char * libname()
	{
		return "u202";
	}
    U202() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U202> pu202;
#endif
