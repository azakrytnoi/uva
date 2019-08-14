#pragma once

class U10092 {
public:
    static const char* libname()
    {
        return "u10092";
    }
    U10092() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10092> pu10092;
#endif
