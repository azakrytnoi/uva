#pragma once

class U207 {
public:
    static const char* libname()
    {
        return "u207";
    }
    U207() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U207> pu207;
#endif
