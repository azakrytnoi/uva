#pragma once

class  U10013
{
public:
    static const char * libname()
    {
        return "u10013";
    }
    U10013();

    void operator()() const;
};
#ifdef POPULATE_CACHE
populate <U10013> pu10013;
#endif

