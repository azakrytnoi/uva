#pragma once

class  U400 {
public:
    static const char* libname()
    {
        return "u400";
    }
    U400();

    void operator()() const;
};
#ifdef POPULATE_CACHE
populate <U400> pu400;
#endif

