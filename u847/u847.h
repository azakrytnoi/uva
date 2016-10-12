#pragma once

class U847
{
public:
    static const char * libname()
    {
        return "u847";
    }
    U847() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U847> pu847;
#endif
