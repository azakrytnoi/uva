#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u210.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <deque>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <memory>
#include <unordered_map>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U210 instance;
    instance();
}

namespace {

    enum class status_t
        : int {
        running, ready, waiting, terminated
    };

    enum class op_type_t
        : int {
        assignment, output, lock, unlock, end
    };

    class processor_t;
    class operator_t {
    public:
        virtual ~operator_t()
        {
        }

        virtual void operator()(processor_t& sys) = 0;
        virtual op_type_t op_type() const = 0;
    };

    class assignment_t: public operator_t {
        char variable_;
        uint16_t value_;

    public:
        assignment_t(const std::string& line) :
            variable_(), value_()
        {
            std::stringstream iss(line);
            std::istream_iterator<std::string> begin(iss), end;
            const std::vector<std::string> tokens (begin, end);
            variable_ = tokens[0][0];
            value_ = std::atoi(tokens[2].c_str());
        }

        virtual void operator ()(processor_t& sys) override;
        virtual op_type_t op_type() const override
        {
            return op_type_t::assignment;
        }

        friend std::ostream& operator << (std::ostream& out, const assignment_t& op)
        {
            out << op.variable_ << " = " << op.value_;
            return out;
        }
    };

    class output_t: public operator_t {
        char variable_;

    public:
        output_t(const std::string& line) :
            variable_()
        {
            std::stringstream iss(line);
            std::istream_iterator<std::string> begin(iss), end;
            std::vector<std::string> tokens (begin, end);
            variable_ = tokens[1][0];
        }

        virtual void operator()(processor_t& sys) override;
        virtual op_type_t op_type() const override
        {
            return op_type_t::output;
        }

        friend std::ostream& operator << (std::ostream& out, const output_t& op)
        {
            out << "print " << op.variable_;
            return out;
        }
    };

    class lock_t: public operator_t {
    public:
        lock_t() { }

        virtual void operator()(processor_t& sys) override;
        virtual op_type_t op_type() const override
        {
            return op_type_t::lock;
        }

        friend std::ostream& operator << (std::ostream& out, const lock_t& /*op*/)
        {
            out << "lock";
            return out;
        }
    };

    class end_t: public operator_t {
    public:
        end_t() { }

        virtual void operator()(processor_t& sys) override;
        virtual op_type_t op_type() const override
        {
            return op_type_t::end;
        }

        friend std::ostream& operator << (std::ostream& out, const end_t& /*op*/)
        {
            out << "end";
            return out;
        }
    };

    class unlock_t: public operator_t {
    public:
        unlock_t() { }

        virtual void operator()(processor_t& sys) override;
        virtual op_type_t op_type() const override
        {
            return op_type_t::unlock;
        }

        friend std::ostream& operator << (std::ostream& out, const unlock_t& /*op*/)
        {
            out << "unlock";
            return out;
        }
    };

    class programm_t {
        status_t status_;
        uint16_t id_;
        std::vector<std::shared_ptr<operator_t>> body_;
        std::vector<std::shared_ptr<operator_t>>::iterator pc_;

    public:
        programm_t(uint16_t id) : status_(status_t::ready), id_(id), body_(), pc_() {}

        void addOperator (const std::string& line);
        uint16_t id() const
        {
            return id_;
        }
        status_t status() const
        {
            return status_;
        }

        std::shared_ptr<operator_t> fetch_instruction ()
        {
            return *pc_++;
        }

        void start()
        {
            status_ = status_t::running;
        }
        void suspend()
        {
            status_ = status_t::waiting;
        }
        void terminate()
        {
            status_ = status_t::terminated;
        }
    };

    class processor_t {
        std::unordered_map<char, int16_t> variables_;
        std::unordered_map<uint16_t, std::shared_ptr<programm_t>> programms_;
        std::unordered_map<op_type_t, uint16_t> timings_;
        uint16_t time_, quantum_;
        uint16_t current_;
        std::deque<uint16_t> locked_;
        std::deque<uint16_t> ready_;
        std::ostream* out_;

    public:
        processor_t() :
            variables_(), programms_(), timings_(), time_(), quantum_(), current_(), locked_(), ready_(), out_()
        {
        }

        processor_t(const processor_t& rhs) = delete;
        processor_t& operator = (const processor_t& rhs) = delete;

        std::unordered_map<uint16_t, std::shared_ptr<programm_t>>& programms()
        {
            return programms_;
        }

        void reset(size_t N, uint16_t assign_time, uint16_t print_time,
                   uint16_t lock_time, uint16_t unlock_time, uint16_t end_time, uint16_t quant)
        {
            time_ = 0;
            quantum_ = quant;
            timings_ = { {op_type_t::assignment, assign_time}, {op_type_t::output, print_time}, {op_type_t::lock, lock_time}, {op_type_t::unlock, unlock_time}, {op_type_t::end, end_time}};
            programms_.clear();
            programms_.reserve(N);
            locked_.clear();
            ready_.clear();
        }

        void setVariable(char name, uint16_t val)
        {
            variables_[name] = val;
        }

        uint16_t getVariable(char name)
        {
            return variables_[name];
        }

        void lockProgram()
        {
            if (not locked_.empty()) {
                programms_[current_]->suspend();
            }

            locked_.push_back(current_);
        }

        void unlockProgram()
        {
            auto currentIdx = std::find(locked_.begin(), locked_.end(), current_);

            if (currentIdx != locked_.end()) {
                locked_.erase(currentIdx);
            }

            if (not locked_.empty()) {
                uint16_t next = locked_.front();
                locked_.pop_front();
                ready_.push_back(next);
            }
        }

        void terminate()
        {
            programms_[current_]->terminate();
        }

        void run(std::ostream& out);

        void print (uint16_t value)
        {
            (*out_) << value << std::endl;
        }

        void execute (operator_t& op)
        {
            (*out_) << current_ << ": ";

            switch (op.op_type()) {
            case op_type_t::assignment:
                (*out_) << static_cast<assignment_t&>(op);
                break;

            case op_type_t::output:
                (*out_) << static_cast<output_t&>(op);
                break;

            case op_type_t::lock:
                (*out_) << static_cast<lock_t&>(op);
                break;

            case op_type_t::unlock:
                (*out_) << static_cast<unlock_t&>(op);
                break;

            case op_type_t::end:
                (*out_) << static_cast<end_t&>(op);
                break;

            default:
                break;
            }

            (*out_) << std::endl;
            op(*this);
            time_ += timings_[op.op_type()];
        }

    };

    class solution {
    public:
        solution() :
            system_()
        {
        }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()(std::ostream& out);

    private:
        processor_t system_;
    };

    std::istream& operator >>(std::istream& in, solution& sol)
    {
        size_t N;
        uint16_t assign_time, print_time, lock_time, unlock_time, end_time;
        uint16_t quantum_time;
        in >> N >> assign_time >> print_time >> lock_time >> unlock_time >> end_time >> quantum_time;
        sol.system_.reset(N, assign_time, print_time, lock_time, unlock_time, end_time, quantum_time);
        std::string line;
        std::getline(in, line);

        while (N--) {
            auto prog = std::make_shared<programm_t>(sol.system_.programms().size() + 1);

            while (std::getline(in, line) && line != "end") {
                prog->addOperator(line);
            }

            prog->addOperator("end");
            sol.system_.programms().insert(std::make_pair(prog->id(), prog)); // @suppress("Invalid arguments")
        }

        return in;
    }

    solution& solution::operator()(std::ostream& out)
    {
        system_.run(out);
        return *this;
    }

    void processor_t::run(std::ostream& out)
    {
        out_ = &out;
        std::transform(programms_.begin(), programms_.end(), std::back_inserter(ready_), [](auto & p) {
            return p.first;
        });
        std::sort(ready_.begin(), ready_.end());

        while (not ready_.empty()) {
            time_ = 0;
            current_ = ready_.front();
            ready_.pop_front();
            programms_[current_]->start();

            while (programms_[current_]->status() == status_t::running && time_ < quantum_) {
                auto instruction = programms_[current_]->fetch_instruction();
                execute(*instruction);
            }

            if (programms_[current_]->status() == status_t::running) {
                ready_.push_back(current_);
            }
        }
    }

    void programm_t::addOperator(const std::string& line)
    {
        if (line == "lock") {
            body_.push_back(std::make_shared<lock_t>());
        } else if (line == "unlock") {
            body_.push_back(std::make_shared<unlock_t>());
        } else if (line == "end") {
            body_.push_back(std::make_shared<end_t>());
        } else if (line.substr(0, line.find(' ')) == "print") {
            body_.push_back(std::make_shared<output_t>(line));
        } else {
            body_.push_back(std::make_shared<assignment_t>(line));
        }

        pc_ = body_.begin();
    }

    void assignment_t::operator ()(processor_t& sys)
    {
        sys.setVariable (variable_, value_);
    }

    void output_t::operator ()(processor_t& sys)
    {
        sys.print(sys.getVariable(variable_));
    }

    void lock_t::operator ()(processor_t& sys)
    {
        sys.lockProgram();
    }

    void unlock_t::operator ()(processor_t& sys)
    {
        sys.unlockProgram();
    }

    void end_t::operator ()(processor_t& sys)
    {
        sys.terminate();
    }
}

void U210::operator()() const
{
    size_t N;
    std::cin >> N;
    solution sol;

    while (N-- != 0 && std::cin >> sol && sol) {
        sol(std::cout);
        std::cout << std::endl;
    }
}
