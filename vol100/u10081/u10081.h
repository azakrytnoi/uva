#pragma once

class U10081 {
public:
    static const char* libname()
    {
        return "u10081";
    }
    U10081() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10081> pu10081;
#endif
