#pragma once

class U206 {
public:
    static const char* libname()
    {
        return "u206";
    }
    U206() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U206> pu206;
#endif
