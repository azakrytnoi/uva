#pragma once

class U11984 {
public:
    static const char* libname()
    {
        return "u11984";
    }
    U11984() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U11984> pu11984;
#endif
