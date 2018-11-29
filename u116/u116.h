#pragma once

class  U116 {
public:
    static const char* libname()
    {
        return "u116";
    }
    U116();

    void operator()() const;
};
#ifdef POPULATE_CACHE
    populate <U116> pu116;
#endif

