#pragma once

class U10267 {
public:
    static const char* libname()
    {
        return "u10267";
    }
    U10267() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10267> pu10267;
#endif
