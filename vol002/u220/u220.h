#pragma once

class U220 {
public:
    static const char* libname()
    {
        return "u220";
    }
    U220() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U220> pu220;
#endif
