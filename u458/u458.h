#pragma once

class U458
{
public:
	static const char * libname()
	{
		return "u458";
	}
    U458() {}

    void operator()();
};
#ifdef POPULATE_CACHE 
populate <U458> pu458; 
#endif 

