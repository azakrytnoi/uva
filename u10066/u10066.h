#pragma once

class  U10066
{
public:
    static const char * libname()
    {
        return "u10066";
    }
    U10066();

    void operator()() const;
};
#ifdef POPULATE_CACHE
populate <U10066> pu10066;
#endif

