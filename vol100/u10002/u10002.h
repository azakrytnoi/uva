#pragma once

class U10002 {
public:
    static const char* libname()
    {
        return "u10002";
    }
    U10002() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10002> pu10002;
#endif
