#pragma once

class U306 {
public:
    static const char* libname()
    {
        return "u306";
    }
    U306() {}

    void operator()() const;
};
#ifdef POPULATE_CACHE
populate <U306> pu306;
#endif

