#pragma once

class U10139 {
public:
    static const char* libname()
    {
        return "u10139";
    }
    U10139() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10139> pu10139;
#endif
