#pragma once

class U10071 {
public:
    static const char* libname()
    {
        return "u10071";
    }
    U10071() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10071> pu10071;
#endif
