#pragma once

class U11111 {
public:
    static const char* libname()
    {
        return "u11111";
    }
    U11111() {}

    void operator()() const;
};
#ifdef POPULATE_CACHE
populate <U11111> pu11111;
#endif
