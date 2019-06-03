#pragma once

class U10024 {
public:
    static const char* libname()
    {
        return "u10024";
    }
    U10024() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10024> pu10024;
#endif
