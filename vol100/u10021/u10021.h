#pragma once

class U10021 {
public:
    static const char* libname()
    {
        return "u10021";
    }
    U10021() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10021> pu10021;
#endif
