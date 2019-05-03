#pragma once

class U10055 {
public:
    static const char* libname()
    {
        return "u10055";
    }
    U10055() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10055> pu10055;
#endif
