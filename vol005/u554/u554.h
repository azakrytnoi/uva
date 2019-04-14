#pragma once

class U554 {
public:
    static const char* libname()
    {
        return "u554";
    }
    U554() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U554> pu554;
#endif
