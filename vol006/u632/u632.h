#pragma once

class U632 {
public:
    static const char* libname()
    {
        return "u632";
    }
    U632() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U632> pu632;
#endif
