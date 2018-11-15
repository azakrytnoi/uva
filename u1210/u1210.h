#pragma once

class U1210 {
public:
    static const char* libname()
    {
        return "u1210";
    }
    U1210() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U1210> pu1210;
#endif
