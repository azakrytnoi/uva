#pragma once

class U10080 {
public:
    static const char* libname()
    {
        return "u10080";
    }
    U10080() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10080> pu10080;
#endif
