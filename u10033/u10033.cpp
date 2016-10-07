#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10033.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cstdlib>

extern "C"
{
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10033 instance;
    instance();
}

namespace
{

class computer
{
    std::vector<uint16_t> registers_;
    std::vector<uint16_t> ram_;
    uint16_t pc_ = 0;
    bool running_ = false;
    static const uint16_t MODULO = 1000;
public:
    computer() :registers_(), ram_()
    {
        registers_.reserve(10);
        ram_.reserve(1000);
    }

    operator bool () const
    {
        return running_;
    }

    friend std::istream& operator >> (std::istream& in, computer& comp)
    {
        comp.reset();
        std::string line;
        auto current = comp.ram_.begin();
        while (std::getline(in, line) && !line.empty()) {
            uint16_t word = std::atoi(line.c_str()) % computer::MODULO;
            *(current++) = word;
        }
        return in;
    }

    void step();

private:
    void reset ()
    {
        registers_.clear();
        registers_.resize(registers_.capacity());
        ram_.clear();
        ram_.resize(ram_.capacity());
        pc_ = 0;
        running_ = true;
    }

    inline void halt()
    {
        running_ = false;
    }

    inline void condJump(uint8_t d, uint8_t s)
    {
        if (registers_[s] != 0) {
            pc_ = registers_[d];
        }
    }

    inline void setRegisterConstant(uint8_t d, uint8_t n)
    {
        registers_[d] = n;
    }

    inline void addRegisterConstant(uint8_t d, uint8_t n)
    {
        uint16_t temp = registers_[d] + n;
        registers_[d] = temp % MODULO;
    }

    inline void multiplyRegisterConstant (uint8_t d, uint8_t n)
    {
        uint16_t temp = registers_[d] * n;
        registers_[d] = temp % MODULO;
    }

    inline void setRegisterRegister(uint8_t d, uint8_t s)
    {
        registers_[d] = registers_[s];
    }

    inline void addRegisterRegister(uint8_t d, uint8_t s)
    {
        uint16_t temp = registers_[d] + registers_[s];
        registers_[d] = temp % MODULO;
    }

    inline void multiplyRegisterRegister(uint8_t d, uint8_t s)
    {
        uint16_t temp = registers_[d] * registers_[s];
        registers_[d] = temp % MODULO;
    }

    inline void setRegisterRam(uint8_t d, uint8_t a)
    {
        registers_[d] = ram_[registers_[a]];
    }

    inline void setRamRegister(uint8_t s, uint8_t a)
    {
        ram_[registers_[a]] = registers_[s];
    }

};

class solution
{
    computer comp_;
    uint32_t steps_ = 0;
public:
    solution() : comp_() {}

    friend std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.comp_;
        sol.steps_ = 0;
        return in;
    }

    friend std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.steps_ << std::endl;
        return out;
    }

    solution& operator () ()
    {
        while (comp_) {
            steps_++;
            comp_.step();
        }
        return *this;
    }
};

void computer::step()
{
    if (running_) {
        uint16_t instruction = ram_[pc_++];
        uint8_t code = instruction / 100;
        uint8_t op1 = (instruction % 100) / 10;
        uint8_t op2 = instruction % 10;
        switch (code) {
        case 0:
            condJump(op1, op2);
            break;
        case 2:
            setRegisterConstant(op1, op2);
            break;
        case 3:
            addRegisterConstant(op1, op2);
            break;
        case 4:
            multiplyRegisterConstant (op1, op2);
            break;
        case 5:
            setRegisterRegister(op1, op2);
            break;
        case 6:
            addRegisterRegister(op1, op2);
            break;
        case 7:
            multiplyRegisterRegister(op1, op2);
            break;
        case 8:
            setRegisterRam(op1, op2);
            break;
        case 9:
            setRamRegister(op1, op2);
            break;
        default:
            halt();
            break;
        }
    }
}

}  // namespace

void U10033::operator()() const
{
    solution sol;
    int N;
    std::string line;
    std::cin >> N;
    std::getline(std::cin, line);
    std::getline(std::cin, line);
    while (N--) {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
