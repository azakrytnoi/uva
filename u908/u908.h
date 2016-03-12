#pragma once
class  U908 {
public:
	static const char * libname()
	{
		return "u908";
	}
	U908();
	~U908();

	void operator()();
};
#ifdef POPULATE_CACHE 
populate <U908> pu908; 
#endif 

