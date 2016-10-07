#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10141.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C"
{
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10141 instance;
    instance();
}

namespace
{

class solution
{
    uint32_t nReq_, nProposals_;
    std::string choosen_;
public:
    solution() : nReq_(0), nProposals_(0), choosen_() {}

    friend std::istream& operator >>(std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol)
    {
        out << sol.choosen_ << std::endl;
        return out;
    }

    operator bool() const
    {
        return nReq_ != 0 && nProposals_ != 0;
    }

    solution& operator()();

private:
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    in >> sol.nReq_ >> sol.nProposals_;
    return in;
}

solution& solution::operator ()()
{
    double compliance = std::numeric_limits<double>::min();
    double minPrice = std::numeric_limits<double>::max();
    std::string line;
    uint32_t temp(nReq_ + 1);
    while (temp--) {
        std::getline(std::cin, line);
    }
    while (nProposals_--) {
        std::getline(std::cin, line);
        double price(0);
        double offer(0);
        std::cin >> price >> offer;
        double comp (offer / nReq_);
        if (comp > compliance) {
            compliance = comp;
            choosen_ = line;
            minPrice = price;
        } else if (std::abs(compliance - comp) <= 1e-6) {
            if (minPrice > price) {
                compliance = comp;
                choosen_ = line;
                minPrice = price;
            }
        }
        uint32_t temp(offer + 1);
        while (temp--) {
            std::getline(std::cin, line);
        }
    }
    return *this;
}

}

void U10141::operator()() const
{
    uint32_t caseNo(0);
    solution sol;
    while (std::cin >> sol && sol) {
        std::cout << "RFP #" << (++caseNo) << std::endl << sol() << std::endl;
    }
}
