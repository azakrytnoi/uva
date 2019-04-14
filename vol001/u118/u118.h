#pragma once

class U118 {
public:
    static const char* libname()
    {
        return "u118";
    }
    U118() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U118> pu118;
#endif
