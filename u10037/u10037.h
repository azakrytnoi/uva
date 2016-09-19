#pragma once

class U10037
{
public:
	static const char * libname()
	{
		return "u10037";
	}
    U10037() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U10037> pu10037;
#endif
