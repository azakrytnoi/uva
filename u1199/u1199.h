#pragma once

class U1199 {
public:
    static const char * libname()
    {
        return "u1199";
    }
    U1199() {}

    void operator()();
};
#ifdef POPULATE_CACHE
populate <U1199> pu1199;
#endif

