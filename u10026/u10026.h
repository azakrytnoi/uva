#pragma once

class U10026 {
public:
    static const char* libname()
    {
        return "u10026";
    }
    U10026() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10026> pu10026;
#endif
