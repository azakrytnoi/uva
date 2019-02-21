#pragma once

class U405
{
public:
	static const char * libname()
	{
		return "u405";
	}
    U405() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U405> pu405;
#endif
