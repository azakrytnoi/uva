#pragma once

class U453 {
public:
    static const char* libname()
    {
        return "u453";
    }
    U453() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U453> pu453;
#endif
