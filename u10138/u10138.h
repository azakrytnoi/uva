#pragma once

class U10138 {
public:
    static const char* libname()
    {
        return "u10138";
    }
    U10138() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10138> pu10138;
#endif
