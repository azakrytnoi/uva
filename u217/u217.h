#pragma once

class U217
{
public:
	static const char * libname()
	{
		return "u217";
	}
    U217() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U217> pu217;
#endif
