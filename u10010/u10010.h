#pragma once

class U10010
{
public:
	static const char * libname()
	{
		return "u10010";
	}
    U10010() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U10010> pu10010;
#endif
