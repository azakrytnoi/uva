#pragma once

class U445
{
public:
	static const char * libname()
	{
		return "u445";
	}
    U445() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U445> pu445;
#endif
