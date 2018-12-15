#pragma once

class U204 {
public:
    static const char* libname()
    {
        return "u204";
    }
    U204() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U204> pu204;
#endif
