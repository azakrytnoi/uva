#pragma once

class U424 {
public:
    static const char* libname()
    {
        return "u424";
    }
    U424() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U424> pu424;
#endif
