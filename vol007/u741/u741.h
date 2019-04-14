#pragma once

class U741 {
public:
    static const char* libname()
    {
        return "u741";
    }
    U741() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U741> pu741;
#endif
