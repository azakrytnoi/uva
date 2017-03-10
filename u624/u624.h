#pragma once
class U624 {
public:
    static const char* libname()
    {
        return "u624";
    }
    U624();
    ~U624();
    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U624> pu624;
#endif

