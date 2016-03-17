#pragma once

class  U478 {
public:
    static const char * libname()
    {
        return "u478";
    }
    U478();

    void operator()();
};

class  U477 : public U478 {
public:
    static const char * libname()
    {
        return "u478";
    }
    U477();
};

class  U476 : public U478 {
public:
    static const char * libname()
    {
        return "u478";
    }
    U476();
};
#ifdef POPULATE_CACHE
populate <U478> pu478;
#endif

