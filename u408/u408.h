#pragma once

class  U408 {
public:
    static const char * libname()
    {
        return "u408";
    }
    U408();
    ~U408();
    void operator()();
};
#ifdef POPULATE_CACHE
populate <U408> pu408;
#endif

