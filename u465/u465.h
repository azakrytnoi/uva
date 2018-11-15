#pragma once

class U465 {
public:
    static const char* libname()
    {
        return "u465";
    }
    U465() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U465> pu465;
#endif
