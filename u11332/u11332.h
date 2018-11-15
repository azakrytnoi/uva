#pragma once

class U11332 {
public:
    static const char* libname()
    {
        return "u11332";
    }
    U11332() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U11332> pu11332;
#endif
