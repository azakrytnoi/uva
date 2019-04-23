#pragma once

class U10023 {
public:
    static const char* libname()
    {
        return "u10023";
    }
    U10023() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10023> pu10023;
#endif
