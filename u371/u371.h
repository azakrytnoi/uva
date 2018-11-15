#pragma once

class U371 {
public:
    static const char* libname()
    {
        return "u371";
    }
    U371() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U371> pu371;
#endif
