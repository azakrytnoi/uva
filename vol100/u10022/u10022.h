#pragma once

class U10022 {
public:
    static const char* libname()
    {
        return "u10022";
    }
    U10022() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10022> pu10022;
#endif
