#pragma once

class U433 {
public:
    static const char* libname()
    {
        return "u433";
    }
    U433() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U433> pu433;
#endif
