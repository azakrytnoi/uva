#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u448.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <limits>
#include <functional>

#include "utility.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U448 instance;
    instance();
}

namespace {

    enum class op_code_t : uint8_t
    {
        ADD = 0, SUB, MUL, DIV, MOV, BREQ, BRLE, BRLS, BRGE, BRGR, BRNE, BR, AND, OR, XOR, NOT
    };

    enum class addr_type_t : uint8_t
    {
        Register = 0, Absolute, PCRelative, Constant
    };

    class instruction_t {
        op_code_t op_code_;
    public:
        explicit instruction_t(op_code_t op) : op_code_(op) {}
        virtual ~instruction_t() {}

        op_code_t& op_code()
        {
            return op_code_;
        }
        op_code_t op_code() const
        {
            return op_code_;
        }

        virtual std::string format() const = 0;

        static std::shared_ptr<instruction_t> decode(std::istream& in, char op);

        friend std::ostream& operator<<(std::ostream& out, const instruction_t& inst)
        {
            out << inst.format() << std::endl;
            return out;
        }
    };

    class op_address_t {
        uint16_t value_;
    public:
        op_address_t(uint16_t val) : value_(val) {}
        virtual ~op_address_t() {}

        uint16_t& value()
        {
            return value_;
        }
        uint16_t value() const
        {
            return value_;
        }

        static std::shared_ptr<op_address_t> decode(std::istream& in);

        virtual std::string format() const = 0;
    };

    class register_t : public op_address_t {
    public:
        register_t(uint16_t val) : op_address_t(val) {}
        ~register_t() {}

        std::string format() const
        {
            std::stringstream temp;
            temp << "R" << value();
            return temp.str();
        }
    };

    class number_t : public op_address_t {
    public:
        number_t(uint16_t val) : op_address_t(val) {}
        ~number_t() {}

        std::string format() const
        {
            std::stringstream temp;
            temp  << value();
            return temp.str();
        }
    };

    class address_t : public op_address_t {
        bool absolute_;
    public:
        address_t(uint16_t val, bool abs) : op_address_t(val), absolute_(abs) {}
        ~address_t() {}

        std::string format() const
        {
            std::stringstream temp;
            temp << (absolute_ ? "$" : "PC+") << value();
            return temp.str();
        }
    };

    class _1_op_t : public instruction_t {
        std::shared_ptr<op_address_t> op1_;
    public:
        _1_op_t(op_code_t op, std::istream& in) : instruction_t(op), op1_()
        {
            op1_ = op_address_t::decode(in);
        }

        virtual ~_1_op_t() {}

        std::string format() const
        {
            std::stringstream out;
            out << op_str() << " " << op1_->format();
            return out.str();
        }

        virtual std::string op_str() const = 0;
    };


    class _2_op_t : public instruction_t {
        std::shared_ptr<op_address_t> op1_;
        std::shared_ptr<op_address_t> op2_;
    public:
        _2_op_t(op_code_t op, std::istream& in) : instruction_t(op), op1_(), op2_()
        {
            op1_ = op_address_t::decode(in);
            op2_ = op_address_t::decode(in);
        }

        virtual ~_2_op_t() {}

        std::string format() const
        {
            std::stringstream out;
            out << op_str() << " " << op1_->format() << ',' << op2_->format();
            return out.str();
        }

        virtual std::string op_str() const = 0;
    };

    class _3_op_t : public instruction_t {
        std::shared_ptr<op_address_t> op1_;
        std::shared_ptr<op_address_t> op2_;
        std::shared_ptr<op_address_t> op3_;
    public:
        _3_op_t(op_code_t op, std::istream& in) : instruction_t(op), op1_(), op2_(), op3_()
        {
            op1_ = op_address_t::decode(in);
            op2_ = op_address_t::decode(in);
            op3_ = op_address_t::decode(in);
        }

        virtual ~_3_op_t() {}

        std::string format() const
        {
            std::stringstream out;
            out << op_str() << " " << op1_->format() << ',' << op2_->format() << ',' << op3_->format();
            return out.str();
        }

        virtual std::string op_str() const = 0;
    };

    class add_t : public _2_op_t {
    public:
        add_t(std::istream& in) : _2_op_t(op_code_t::ADD, in) {}
        ~add_t() {}
        std::string op_str() const
        {
            return "ADD";
        }
    };

    class sub_t : public _2_op_t {
    public:
        sub_t(std::istream& in) : _2_op_t(op_code_t::SUB, in) {}
        ~sub_t() {}
        std::string op_str() const
        {
            return "SUB";
        }
    };

    class mul_t : public _2_op_t {
    public:
        mul_t(std::istream& in) : _2_op_t(op_code_t::MUL, in) {}
        ~mul_t() {}
        std::string op_str() const
        {
            return "MUL";
        }
    };

    class div_t : public _2_op_t {
    public:
        div_t(std::istream& in) : _2_op_t(op_code_t::DIV, in) {}
        ~div_t() {}
        std::string op_str() const
        {
            return "DIV";
        }
    };

    class mov_t : public _2_op_t {
    public:
        mov_t(std::istream& in) : _2_op_t(op_code_t::MOV, in) {}
        ~mov_t() {}
        std::string op_str() const
        {
            return "MOV";
        }
    };

    class breq_t : public _1_op_t {
    public:
        breq_t(std::istream& in) : _1_op_t(op_code_t::BREQ, in) {}
        ~breq_t() {}
        std::string op_str() const
        {
            return "BREQ";
        }
    };

    class brle_t : public _1_op_t {
    public:
        brle_t(std::istream& in) : _1_op_t(op_code_t::BRLE, in) {}
        ~brle_t() {}
        std::string op_str() const
        {
            return "BRLE";
        }
    };

    class brls_t : public _1_op_t {
    public:
        brls_t(std::istream& in) : _1_op_t(op_code_t::BRLS, in) {}
        ~brls_t() {}
        std::string op_str() const
        {
            return "BRLS";
        }
    };

    class brge_t : public _1_op_t {
    public:
        brge_t(std::istream& in) : _1_op_t(op_code_t::BRGE, in) {}
        ~brge_t() {}
        std::string op_str() const
        {
            return "BRGE";
        }
    };

    class brgr_t : public _1_op_t {
    public:
        brgr_t(std::istream& in) : _1_op_t(op_code_t::BRGR, in) {}
        ~brgr_t() {}
        std::string op_str() const
        {
            return "BRGR";
        }
    };

    class brne_t : public _1_op_t {
    public:
        brne_t(std::istream& in) : _1_op_t(op_code_t::BRNE, in) {}
        ~brne_t() {}
        std::string op_str() const
        {
            return "BRNE";
        }
    };

    class br_t : public _1_op_t {
    public:
        br_t(std::istream& in) : _1_op_t(op_code_t::BR, in) {}
        ~br_t() {}
        std::string op_str() const
        {
            return "BR";
        }
    };

    class not_t : public _1_op_t {
    public:
        not_t(std::istream& in) : _1_op_t(op_code_t::NOT, in) {}
        ~not_t() {}
        std::string op_str() const
        {
            return "NOT";
        }
    };

    class and_t : public _3_op_t {
    public:
        and_t(std::istream& in) : _3_op_t(op_code_t::AND, in) {}
        ~and_t() {}
        std::string op_str() const
        {
            return "AND";
        }
    };

    class or_t : public _3_op_t {
    public:
        or_t(std::istream& in) : _3_op_t(op_code_t::OR, in) {}
        ~or_t() {}
        std::string op_str() const
        {
            return "OR";
        }
    };

    class xor_t : public _3_op_t {
    public:
        xor_t(std::istream& in) : _3_op_t(op_code_t::XOR, in) {}
        ~xor_t() {}
        std::string op_str() const
        {
            return "XOR";
        }
    };

    class solution_t {
    public:
        solution_t() : bin_(), code_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return true;
        }
        solution_t& operator()();

    private:
        std::string bin_;
        std::vector<std::shared_ptr<instruction_t>> code_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        std::string line;

        while (std::getline(in, line))
        {
            sol.bin_ += line;
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        std::for_each(sol.code_.begin(), sol.code_.end(), [&](const std::shared_ptr<instruction_t> instruction)
        {
            out << instruction->format() << std::endl;
        });
        return out;
    }

    solution_t& solution_t::operator()()
    {
        code_.reserve(bin_.length() / 5 + 1);
        std::stringstream in(bin_);
        char op(0);

        while (in >> op)
        {
            auto inst(instruction_t::decode(in, op));
            code_.push_back(inst);
        }

        return *this;
    }

    std::shared_ptr<instruction_t> instruction_t::decode(std::istream& in, char op)
    {
        static const std::map<op_code_t, std::function<std::shared_ptr<instruction_t>(std::istream&)>> decoder(
        {
            {op_code_t::ADD, [&](std::istream & in)
                {
                    return std::make_shared<add_t>(in);
                }
            },
            {op_code_t::SUB, [&](std::istream & in)
                {
                    return std::make_shared<sub_t>(in);
                }
            },
            {op_code_t::MUL, [&](std::istream & in)
                {
                    return std::make_shared<mul_t>(in);
                }
            },
            {op_code_t::DIV, [&](std::istream & in)
                {
                    return std::make_shared<div_t>(in);
                }
            },
            {op_code_t::MOV, [&](std::istream & in)
                {
                    return std::make_shared<mov_t>(in);
                }
            },
            {op_code_t::BREQ, [&](std::istream & in)
                {
                    return std::make_shared<breq_t>(in);
                }
            },
            {op_code_t::BRLE, [&](std::istream & in)
                {
                    return std::make_shared<brle_t>(in);
                }
            },
            {op_code_t::BRLS, [&](std::istream & in)
                {
                    return std::make_shared<brls_t>(in);
                }
            },
            {op_code_t::BRGE, [&](std::istream & in)
                {
                    return std::make_shared<brge_t>(in);
                }
            },
            {op_code_t::BRGR, [&](std::istream & in)
                {
                    return std::make_shared<brge_t>(in);
                }
            },
            {op_code_t::BRNE, [&](std::istream & in)
                {
                    return std::make_shared<brne_t>(in);
                }
            },
            {op_code_t::BR, [&](std::istream & in)
                {
                    return std::make_shared<br_t>(in);
                }
            },
            {op_code_t::AND, [&](std::istream & in)
                {
                    return std::make_shared<and_t>(in);
                }
            },
            {op_code_t::OR, [&](std::istream & in)
                {
                    return std::make_shared<or_t>(in);
                }
            },
            {op_code_t::XOR, [&](std::istream & in)
                {
                    return std::make_shared<xor_t>(in);
                }
            },
            {op_code_t::NOT, [&](std::istream & in)
                {
                    return std::make_shared<not_t>(in);
                }
            }
        });
        op = op >= 'A' ? 10 + op - 'A' : op - '0';

        auto opi(decoder.find(static_cast<op_code_t>(op)));

        if (opi == decoder.end())
        {
            return nullptr;
        }

        return opi->second(in);
    }

    std::shared_ptr<op_address_t> op_address_t::decode(std::istream& in)
    {
        uint16_t raw_op(0);
        std::istream_iterator<char> iin(in);

        std::copy_n(iin, 4, hex_translator_t<uint16_t>(raw_op));

        uint16_t op_value(raw_op & 0x3fff);
        uint8_t op_type((raw_op >> 14) & 0x03);

        switch (static_cast<addr_type_t>(op_type))
        {
        case addr_type_t::Register:
            return std::make_shared<register_t>(op_value);

        case addr_type_t::Absolute:
            return std::make_shared<address_t>(op_value, true);

        case addr_type_t::PCRelative:
            return std::make_shared<address_t>(op_value, false);

        case addr_type_t::Constant:
            return std::make_shared<number_t>(op_value);

        default:
            return nullptr;
        }
    }
}

void U448::operator()() const
{
    solution_t sol;

    std::cin >> sol;
    std::cout << sol();
}
