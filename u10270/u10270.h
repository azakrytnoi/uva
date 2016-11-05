#pragma once

class U10270
{
public:
	static const char * libname()
	{
		return "u10270";
	}
    U10270() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10270> pu10270;
#endif
