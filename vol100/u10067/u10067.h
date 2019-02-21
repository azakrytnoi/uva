#pragma once

class U10067 {
public:
    static const char* libname()
    {
        return "u10067";
    }
    U10067() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10067> pu10067;
#endif
