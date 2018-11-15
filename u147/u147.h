#pragma once

class U147 {
public:
    static const char* libname()
    {
        return "u147";
    }
    U147() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U147> pu147;
#endif
