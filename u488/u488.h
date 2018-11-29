#pragma once

class U488 {
public:
    static const char* libname()
    {
        return "u488";
    }
    U488() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U488> pu488;
#endif
