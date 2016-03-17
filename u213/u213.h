#pragma once

class U213 {
public:
    static const char * libname()
    {
        return "u213";
    }
    U213() {}

    void operator()();
};
#ifdef POPULATE_CACHE
populate <U213> pu213;
#endif

