#pragma once
class  U908 {
public:
    static const char* libname()
    {
        return "u908";
    }
    U908();
    ~U908();

    void operator()() const;
};
#ifdef POPULATE_CACHE
    populate <U908> pu908;
#endif

