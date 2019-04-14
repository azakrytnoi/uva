#pragma once

class U418 {
public:
    static const char* libname()
    {
        return "u418";
    }
    U418() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U418> pu418;
#endif
