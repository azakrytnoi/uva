#pragma once

class U10028 {
public:
    static const char* libname()
    {
        return "u10028";
    }
    U10028() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10028> pu10028;
#endif
