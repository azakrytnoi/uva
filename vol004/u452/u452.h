#pragma once

class U452 {
public:
    static const char* libname()
    {
        return "u452";
    }
    U452() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U452> pu452;
#endif
