#pragma once

class U10152 {
public:
    static const char* libname()
    {
        return "u10152";
    }
    U10152() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10152> pu10152;
#endif
