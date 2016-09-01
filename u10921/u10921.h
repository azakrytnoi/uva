#pragma once

class U10921
{
public:
    static const char * libname()
    {
        return "u10921";
    }
    U10921() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10921> pu10921;
#endif
