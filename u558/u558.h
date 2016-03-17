#pragma once
class  U558 {
public:
    static const char * libname()
    {
        return "u558";
    }
    U558();
    ~U558();
    void operator()();

private:
    static const int Infinity = 10000;
};
#ifdef POPULATE_CACHE
populate <U558> pu558;
#endif

