#pragma once

class U10205
{
public:
	static const char * libname()
	{
		return "u10205";
	}
    U10205() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10205> pu10205;
#endif
