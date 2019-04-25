#pragma once

class U10027 {
public:
    static const char* libname()
    {
        return "u10027";
    }
    U10027() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10027> pu10027;
#endif
