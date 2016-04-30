#pragma once

class U10033
{
public:
    static const char * libname()
    {
        return "u10033";
    }
    U10033() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10033> pu10033;
#endif
