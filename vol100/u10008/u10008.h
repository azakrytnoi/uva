#pragma once

class U10008 {
public:
    static const char* libname()
    {
        return "u10008";
    }
    U10008() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10008> pu10008;
#endif
