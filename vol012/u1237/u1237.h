#pragma once

class U1237
{
public:
	static const char * libname()
	{
		return "u1237";
	}
    U1237() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U1237> pu1237;
#endif
