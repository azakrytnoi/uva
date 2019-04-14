#pragma once

class U216 {
public:
    static const char* libname()
    {
        return "u216";
    }
    U216() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U216> pu216;
#endif
