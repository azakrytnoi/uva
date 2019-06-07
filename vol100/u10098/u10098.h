#pragma once

class U10098 {
public:
    static const char* libname()
    {
        return "u10098";
    }
    U10098() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10098> pu10098;
#endif
