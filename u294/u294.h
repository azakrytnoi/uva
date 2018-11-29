#pragma once

class U294 {
public:
    static const char* libname()
    {
        return "u294";
    }
    U294() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U294> pu294;
#endif
