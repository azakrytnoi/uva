#pragma once

class U222
{
public:
	static const char * libname()
	{
		return "u222";
	}
    U222() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U222> pu222;
#endif
