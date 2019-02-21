#pragma once

class U10051 {
public:
    static const char* libname()
    {
        return "u10051";
    }
    U10051() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10051> pu10051;
#endif
