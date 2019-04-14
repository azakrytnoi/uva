#pragma once

class U10015 {
public:
    static const char* libname()
    {
        return "u10015";
    }
    U10015() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10015> pu10015;
#endif
