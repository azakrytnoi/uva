#pragma once

class U10025 {
public:
    static const char* libname()
    {
        return "u10025";
    }
    U10025() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10025> pu10025;
#endif
