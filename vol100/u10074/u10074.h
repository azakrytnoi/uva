#pragma once

class U10074 {
public:
    static const char* libname()
    {
        return "u10074";
    }
    U10074() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10074> pu10074;
#endif
