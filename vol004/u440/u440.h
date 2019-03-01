#pragma once

class U440
{
public:
	static const char * libname()
	{
		return "u440";
	}
    U440() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U440> pu440;
#endif
