#pragma once

class  U628 {
public:
    static const char* libname()
    {
        return "u628";
    }
    U628();
    void operator()() const;
};
#ifdef POPULATE_CACHE
    populate <U628> pu628;
#endif

