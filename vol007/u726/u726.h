#pragma once

class U726 {
public:
    static const char* libname()
    {
        return "u726";
    }
    U726() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U726> pu726;
#endif
