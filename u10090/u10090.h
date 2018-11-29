#pragma once

class U10090 {
public:
    static const char* libname()
    {
        return "u10090";
    }
    U10090() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10090> pu10090;
#endif
