#pragma once

class U442 {
public:
    static const char* libname()
    {
        return "u442";
    }
    U442() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U442> pu442;
#endif
