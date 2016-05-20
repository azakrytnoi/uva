#pragma once

class  U305
{
public:
    static const char * libname()
    {
        return "u305";
    }
    U305();

    void operator()();
};
#ifdef POPULATE_CACHE
populate <U305> pu305;
#endif

