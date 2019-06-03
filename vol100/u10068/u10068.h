#pragma once

class U10068 {
public:
    static const char* libname()
    {
        return "u10068";
    }
    U10068() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10068> pu10068;
#endif
