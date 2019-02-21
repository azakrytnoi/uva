#pragma once

class U856
{
public:
	static const char * libname()
	{
		return "u856";
	}
    U856() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U856> pu856;
#endif
