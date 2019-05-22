#pragma once

class U10085 {
public:
    static const char* libname()
    {
        return "u10085";
    }
    U10085() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10085> pu10085;
#endif
