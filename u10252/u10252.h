#pragma once

class U10252
{
public:
    static const char * libname()
    {
        return "u10252";
    }
    U10252() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10252> pu10252;
#endif
