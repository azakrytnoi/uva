#pragma once

class U484 {
public:
    static const char* libname()
    {
        return "u484";
    }
    U484() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U484> pu484;
#endif
