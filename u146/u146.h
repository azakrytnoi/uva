#pragma once

class  U146 {
public:
    static const char* libname()
    {
        return "u146";
    }
    U146();
    ~U146();
    void operator()() const;
};
#ifdef POPULATE_CACHE
populate <U146> pu146;
#endif

