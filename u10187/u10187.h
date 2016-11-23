#pragma once

class U10187 {
public:
    static const char* libname()
    {
        return "u10187";
    }
    U10187() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10187> pu10187;
#endif
