#pragma once

class U11541 {
public:
    static const char* libname()
    {
        return "u11541";
    }
    U11541() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U11541> pu11541;
#endif
