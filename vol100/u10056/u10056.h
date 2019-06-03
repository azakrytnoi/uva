#pragma once

class U10056 {
public:
    static const char* libname()
    {
        return "u10056";
    }
    U10056() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10056> pu10056;
#endif
