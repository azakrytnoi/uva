#pragma once

class U10084 {
public:
    static const char* libname()
    {
        return "u10084";
    }
    U10084() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10084> pu10084;
#endif
