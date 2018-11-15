#pragma once

class U10183 {
public:
    static const char* libname()
    {
        return "u10183";
    }
    U10183() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10183> pu10183;
#endif
