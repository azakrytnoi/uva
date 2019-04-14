#pragma once

class U446 {
public:
    static const char* libname()
    {
        return "u446";
    }
    U446() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U446> pu446;
#endif
