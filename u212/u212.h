#pragma once

class U212
{
public:
	static const char * libname()
	{
		return "u212";
	}
    U212() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U212> pu212;
#endif
