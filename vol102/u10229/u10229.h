#pragma once

class U10229 {
public:
    static const char* libname()
    {
        return "u10229";
    }
    U10229() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10229> pu10229;
#endif
