#pragma once

class U10020 {
public:
    static const char* libname()
    {
        return "u10020";
    }
    U10020() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10020> pu10020;
#endif
