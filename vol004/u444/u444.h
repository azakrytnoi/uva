#pragma once

class U444 {
public:
    static const char* libname()
    {
        return "u444";
    }
    U444() {}

    void operator()() const;
};
#ifdef POPULATE_CACHE
    populate <U444> pu444;
#endif

