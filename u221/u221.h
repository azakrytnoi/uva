#pragma once

class U221
{
public:
	static const char * libname()
	{
		return "u221";
	}
    U221() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U221> pu221;
#endif
