#pragma once

class U10044 {
public:
    static const char* libname()
    {
        return "u10044";
    }
    U10044() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10044> pu10044;
#endif
