#pragma once

class U10113 {
public:
    static const char* libname()
    {
        return "u10113";
    }
    U10113() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10113> pu10113;
#endif
