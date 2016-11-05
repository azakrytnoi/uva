#pragma once

class U10041 {
public:
    static const char* libname()
    {
        return "u10041";
    }
    U10041() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10041> pu10041;
#endif
