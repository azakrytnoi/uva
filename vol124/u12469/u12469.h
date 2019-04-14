#pragma once

class U12469 {
public:
    static const char* libname()
    {
        return "u12469";
    }
    U12469() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U12469> pu12469;
#endif
