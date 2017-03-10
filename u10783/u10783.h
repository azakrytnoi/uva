#pragma once

class U10783 {
public:
    static const char* libname()
    {
        return "u10783";
    }
    U10783() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10783> pu10783;
#endif
