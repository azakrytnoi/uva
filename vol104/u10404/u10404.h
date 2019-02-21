#pragma once

class U10404
{
public:
	static const char * libname()
	{
		return "u10404";
	}
    U10404() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10404> pu10404;
#endif
