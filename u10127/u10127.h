#pragma once

class U10127 {
public:
    static const char* libname()
    {
        return "u10127";
    }
    U10127() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10127> pu10127;
#endif
