#pragma once

class U10047 {
public:
    static const char* libname()
    {
        return "u10047";
    }
    U10047() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10047> pu10047;
#endif
