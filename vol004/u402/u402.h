#pragma once

class U402 {
public:
    static const char* libname()
    {
        return "u402";
    }
    U402() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U402> pu402;
#endif
