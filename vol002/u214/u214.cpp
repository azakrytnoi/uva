#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u214.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <stack>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <memory>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U214 instance;
    instance();
}

namespace {

    struct node_t
    {
        std::string name_;
        char op_;
        std::shared_ptr<node_t> left_;
        std::shared_ptr<node_t> right_;
        std::shared_ptr<node_t> parent_;

        node_t (const std::string& name, char op) : name_(name), op_(op), left_(), right_(), parent_() {}

        friend std::ostream& operator << (std::ostream& out, const node_t& node)
        {
            static int ident(0);
            out << std::setw(++ident) << ' ' << std::setw(1) << "node [name: '" << node.name_ << "', operation: '" << node.op_ << "']";

            if (node.left_)
            {
                out << std:: endl << *node.left_;
            }

            if (node.right_)
            {
                out << std::endl << *node.right_;
            }

            --ident;
            return out;
        }
    };

    class parser_t {
        std::string::const_reverse_iterator ptr_;
    public:
        static std::shared_ptr<node_t> parse (const std::string& line);

    private:
        parser_t(const std::string& line) : ptr_(line.rbegin()) {}

        std::shared_ptr<node_t> term();
        std::shared_ptr<node_t> operation();
    };

    class solution {
        std::shared_ptr<node_t> root_;
        std::string compile_;
        bool accu_loaded_;
        bool op_done_;
    public:
        solution() : root_(), compile_(), accu_loaded_(false), op_done_(false) { }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return (bool)root_;
        }
        solution& operator()();

    private:
        std::shared_ptr<node_t> traverse(std::shared_ptr<node_t> node, std::shared_ptr<node_t>& acu, std::stack<int>& temp, std::ostream& out);
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.root_.reset();
        sol.compile_.clear();
        std::string line;

        if (std::getline(in, line))
        {
            sol.root_ = parser_t::parse(line);
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.compile_;
        return out;
    }

    solution& solution::operator()()
    {
        if (root_)
        {
            std::stack<int> temp;
            temp.push(0);
            std::stringstream out;
            std::shared_ptr<node_t> acu;
            traverse(root_, acu, temp, out);
            compile_ = out.str();
        }

        return *this;
    }

    std::shared_ptr<node_t> parser_t::parse(const std::string& line)
    {
        parser_t parser(line);
        return parser.term();
    }

    std::shared_ptr<node_t> parser_t::term()
    {
        auto ch = *ptr_++;

        switch (ch)
        {
        case '+':
        case '-':
        case '*':
        case '/':
        {
            auto node = std::make_shared<node_t>("", ch);
            node->right_ = term();
            node->left_ = term();
            node->left_->parent_ = node->right_->parent_ = node;
            return node;
        }

        case '@':
        {
            auto node = std::make_shared<node_t>("", ch);
            node->left_ = term();
            node->left_->parent_ = node;
            return node;
        }

        default:
            return std::make_shared<node_t>(std::string(1, ch), ' ');
        }
    }

    /*
     *
        if (node) {
            if (node->left_) {
                traverse(node->left_, out);
            }

            if (node->right_) {
                traverse(node->right_, out);
            }

            switch (node->op_) {
            case ' ':
                if (not accu_loaded_) {
                    out << "L " << node->name_ << std::endl;
                    accu_loaded_ = true;
                } else if (op_done_) {
                    auto tmp = temp_.top();
                    temp_.push(++tmp);
                    out << "ST $" << tmp << std::endl << "L " << node->name_ << std::endl;
                    op_done_ = false;
                }

                break;

            case '+':
                if (not op_done_) {
                    out << "A " << node->right_->name_ << std::endl;
                } else {
                    auto tmp = temp_.top();
                    temp_.pop();
                    out << "A $" << tmp << std::endl;
                }

                op_done_ = true;
                break;

            case '-':
                if (op_done_) {
                    auto tmp = temp_.top();
                    temp_.pop();
                    out << "N" << std::endl << "A $" << tmp << std::endl;
                } else {
                    out << "S " << node->right_->name_ << std::endl;
                }

                op_done_ = true;
                break;

            case '*':
                if (not op_done_) {
                    out << "M " << node->right_->name_ << std::endl;
                } else {
                    auto tmp = temp_.top();
                    temp_.pop();
                    out << "M $" << tmp << std::endl;
                }

                op_done_ = true;
                break;

            case '@': {
                auto tmp = temp_.top();
                ++tmp;
                out << "ST $" << tmp << std::endl << "L " << node->left_->name_ << std::endl << "N" << std::endl;
            }

            op_done_ = true;
            break;

            case '/': {
                auto tmp = temp_.top();
                tmp++;
                out << "ST $" << tmp << std::endl << "L " << node->right_->name_ << std::endl << "D $" << tmp << std::endl;
            }

            op_done_ = true;
            break;

            default:
                break;
            }
        }

     */
    std::shared_ptr<node_t> solution::traverse(std::shared_ptr<node_t> node, std::shared_ptr<node_t>& acu, std::stack<int>& temp, std::ostream& out)
    {
        if (node)
        {
            switch (node->op_)
            {
            case ' ':
            {
                if (acu && acu.get() != node.get())
                {
                    auto tmp = temp.top();
                    temp.push(++tmp);
                    out << "ST $" << tmp << std::endl;
                }

                out << "L " << node->name_ << std::endl;
                acu = node;
                break;
            }

            case '+':
            {
                traverse(node->left_, acu, temp, out);

                if (node->right_->op_ == ' ')
                {
                    out << "A " << node->right_->name_ << std::endl;
                }
                else
                {
                    traverse(node->right_, acu, temp, out);
                    auto tmp = temp.top();
                    temp.pop();
                    out << "A $" << tmp << std::endl;
                }

                acu = node;
                break;
            }

            case '-':
            {
                traverse(node->left_, acu, temp, out);

                if (node->right_->op_ == ' ')
                {
                    out << "S " << node->right_->name_ << std::endl;
                }
                else
                {
                    traverse(node->right_, acu, temp, out);
                    auto tmp = temp.top();
                    temp.pop();
                    out << "N" << std::endl << "A $" << tmp << std::endl;
                }

                acu = node;

                break;
            }

            case '@':
            {
                traverse(node->left_, acu, temp, out);
                out << "N" << std::endl;
                acu = node;
                break;
            }

            case '*':
            {
                traverse(node->left_, acu, temp, out);

                if (node->right_->op_ == ' ')
                {
                    out << "M " << node->right_->name_ << std::endl;
                }
                else
                {
                    traverse(node->right_, acu, temp, out);
                    auto tmp = temp.top();
                    temp.pop();
                    out << "M $" << tmp << std::endl;
                }

                acu = node;
                break;
            }

            case '/':
            {
                traverse(node->left_, acu, temp, out);

                if (node->right_->op_ == ' ')
                {
                    out << "D " << node->right_->op_ << std::endl;
                }
                else
                {
                    traverse(node->right_, acu, temp, out);
                    auto tmp = temp.top();
                    temp.pop();
                    out << "ST $" << (tmp + 1) << std::endl << "L $" << tmp << std::endl << "D $" << (tmp + 1) << std::endl;
                }

                break;
            }

            default:
                throw "not implemented";
            }

            return node;
        }

        throw "logic error";
    }
}

void U214::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
