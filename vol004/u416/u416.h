#pragma once

class U416 {
public:
    static const char* libname()
    {
        return "u416";
    }
    U416() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U416> pu416;
#endif
