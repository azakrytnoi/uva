#pragma once

class U704 {
public:
    static const char* libname()
    {
        return "u704";
    }
    U704() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U704> pu704;
#endif
