#pragma once

class U12709
{
public:
	static const char * libname()
	{
		return "u12709";
	}
    U12709() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U12709> pu12709;
#endif
