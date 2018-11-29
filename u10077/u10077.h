#pragma once

class U10077 {
public:
    static const char* libname()
    {
        return "u10077";
    }
    U10077() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10077> pu10077;
#endif
