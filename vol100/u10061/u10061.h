#pragma once

class U10061 {
public:
    static const char* libname()
    {
        return "u10061";
    }
    U10061() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10061> pu10061;
#endif
