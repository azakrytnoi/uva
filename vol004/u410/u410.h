#pragma once

class U410 {
public:
    static const char* libname()
    {
        return "u410";
    }
    U410() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U410> pu410;
#endif
