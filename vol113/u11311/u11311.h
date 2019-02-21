#pragma once

class U11311
{
public:
	static const char * libname()
	{
		return "u11311";
	}
    U11311() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U11311> pu11311;
#endif
