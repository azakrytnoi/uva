#pragma once

class U10970 {
public:
    static const char* libname()
    {
        return "u10970";
    }
    U10970() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10970> pu10970;
#endif
