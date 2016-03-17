#pragma once

class U10878 {
public:
    static const char * libname()
    {
        return "u10878";
    }
    U10878() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U10878> pu10878;
#endif
