#pragma once

class U10110 {
public:
    static const char* libname()
    {
        return "u10110";
    }
    U10110() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10110> pu10110;
#endif
