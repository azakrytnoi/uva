#pragma once

class U10132
{
public:
    static const char * libname()
    {
        return "u10132";
    }
    U10132() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10132> pu10132;
#endif
