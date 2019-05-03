#pragma once

class U10057 {
public:
    static const char* libname()
    {
        return "u10057";
    }
    U10057() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10057> pu10057;
#endif
