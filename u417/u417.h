#pragma once

class  U417 {
public:
    static const char* libname()
    {
        return "u417";
    }
    U417();
    void operator()() const;
};
#ifdef POPULATE_CACHE
    populate <U417> pu417;
#endif

