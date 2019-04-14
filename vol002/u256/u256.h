#pragma once

class U256 {
public:
    static const char* libname()
    {
        return "u256";
    }
    U256() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U256> pu256;
#endif
