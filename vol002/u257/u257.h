#pragma once

class U257 {
public:
    static const char* libname()
    {
        return "u257";
    }
    U257() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U257> pu257;
#endif
