#pragma once

class U209 {
public:
    static const char* libname()
    {
        return "u209";
    }
    U209() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U209> pu209;
#endif
