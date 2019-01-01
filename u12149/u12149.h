#pragma once

class U12149 {
public:
    static const char* libname()
    {
        return "u12149";
    }
    U12149() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U12149> pu12149;
#endif
