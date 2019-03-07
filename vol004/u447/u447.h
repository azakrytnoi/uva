#pragma once

class U447
{
public:
	static const char * libname()
	{
		return "u447";
	}
    U447() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U447> pu447;
#endif
