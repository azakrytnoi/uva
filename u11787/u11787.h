#pragma once

class U11787
{
public:
	static const char * libname()
	{
		return "u11787";
	}
    U11787() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U11787> pu11787;
#endif
