#pragma once

class U568 {
public:
    static const char* libname()
    {
        return "u568";
    }
    U568() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U568> pu568;
#endif
