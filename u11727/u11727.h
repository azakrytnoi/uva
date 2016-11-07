#pragma once

class U11727
{
public:
	static const char * libname()
	{
		return "u11727";
	}
    U11727() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U11727> pu11727;
#endif
