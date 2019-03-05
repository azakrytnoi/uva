#pragma once

class U443
{
public:
	static const char * libname()
	{
		return "u443";
	}
    U443() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U443> pu443;
#endif
