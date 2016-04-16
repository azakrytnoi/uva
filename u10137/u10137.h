#pragma once

class U10137
{
public:
	static const char * libname()
	{
		return "u10137";
	}
    U10137() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10137> pu10137;
#endif
