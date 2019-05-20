#pragma once

class U10082 {
public:
    static const char* libname()
    {
        return "u10082";
    }
    U10082() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10082> pu10082;
#endif
