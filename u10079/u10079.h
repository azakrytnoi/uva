#pragma once

class U10079 {
public:
    static const char* libname()
    {
        return "u10079";
    }
    U10079() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10079> pu10079;
#endif
