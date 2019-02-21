#pragma once

class U542
{
public:
	static const char * libname()
	{
		return "u542";
	}
    U542() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U542> pu542;
#endif
