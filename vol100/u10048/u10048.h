#pragma once

class U10048 {
public:
    static const char* libname()
    {
        return "u10048";
    }
    U10048() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10048> pu10048;
#endif
