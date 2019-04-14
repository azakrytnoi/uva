#pragma once

class U419 {
public:
    static const char* libname()
    {
        return "u419";
    }
    U419() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U419> pu419;
#endif
