#pragma once

class U10040 {
public:
    static const char* libname()
    {
        return "u10040";
    }
    U10040() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10040> pu10040;
#endif
