#pragma once

class U468 {
public:
    static const char * libname()
    {
        return "u468";
    }
    U468() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U468> pu468;
#endif
