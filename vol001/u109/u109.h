#pragma once

class U109 {
public:
    static const char* libname()
    {
        return "u109";
    }
    U109() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U109> pu109;
#endif
