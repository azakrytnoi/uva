#pragma once

class U214 {
public:
    static const char* libname()
    {
        return "u214";
    }
    U214() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U214> pu214;
#endif
