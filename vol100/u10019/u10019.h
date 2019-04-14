#pragma once

class U10019 {
public:
    static const char* libname()
    {
        return "u10019";
    }
    U10019() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10019> pu10019;
#endif
