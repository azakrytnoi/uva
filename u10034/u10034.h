#pragma once

class U10034 {
public:
    static const char* libname()
    {
        return "u10034";
    }
    U10034() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10034> pu10034;
#endif
