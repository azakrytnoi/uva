#pragma once

class U701 {
public:
    static const char* libname()
    {
        return "u701";
    }
    U701() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U701> pu701;
#endif
