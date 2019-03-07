#pragma once

class U448
{
public:
	static const char * libname()
	{
		return "u448";
	}
    U448() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U448> pu448;
#endif
