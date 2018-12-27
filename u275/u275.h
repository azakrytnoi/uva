#pragma once

class U275
{
public:
	static const char * libname()
	{
		return "u275";
	}
    U275() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U275> pu275;
#endif
