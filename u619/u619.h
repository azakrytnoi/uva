#pragma once

class U619 {
public:
    static const char* libname()
    {
        return "u619";
    }
    U619() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U619> pu619;
#endif
