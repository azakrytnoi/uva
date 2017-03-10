#pragma once

class U10029 {
public:
    static const char* libname()
    {
        return "u10029";
    }
    U10029() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10029> pu10029;
#endif
