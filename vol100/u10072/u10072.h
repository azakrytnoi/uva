#pragma once

class U10072 {
public:
    static const char* libname()
    {
        return "u10072";
    }
    U10072() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10072> pu10072;
#endif
