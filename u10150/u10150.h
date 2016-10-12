#pragma once

class U10150
{
public:
    static const char * libname()
    {
        return "u10150";
    }
    U10150() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10150> pu10150;
#endif
