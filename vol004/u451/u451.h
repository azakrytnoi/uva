#pragma once

class U450 {
public:
    static const char* libname()
    {
        return "u450";
    }
    U450() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U450> pu450;
#endif
