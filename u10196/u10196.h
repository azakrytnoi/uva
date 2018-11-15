#pragma once

class U10196 {
public:
    static const char* libname()
    {
        return "u10196";
    }
    U10196() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10196> pu10196;
#endif
