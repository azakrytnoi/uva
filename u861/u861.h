#pragma once

class U861
{
public:
	static const char * libname()
	{
		return "u861";
	}
    U861() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U861> pu861;
#endif
