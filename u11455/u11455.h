#pragma once

class U11455
{
public:
	static const char * libname()
	{
		return "u11455";
	}
    U11455() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U11455> pu11455;
#endif
