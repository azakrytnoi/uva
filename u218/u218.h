#pragma once

class U218 {
public:
    static const char* libname()
    {
        return "u218";
    }
    U218() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U218> pu218;
#endif
