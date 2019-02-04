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
    enum class status_t {
        running, locked, terminated
    };

    enum class op_type_t : char {
        assign, print, lock, unlock, end
    };

    typedef uint64_t time_t;

    class processor_t;
    class instruction_t {
    public:
        instruction_t() {}
        static std::shared_ptr<instruction_t> parse(const std::string& src);

        friend class processor_t;
        virtual op_type_t op_type() const = 0;
        virtual void populate(std::istream& in) = 0;
        virtual std::string to_string() = 0;

        virtual ~instruction_t() {}

        time_t exec(processor_t& processor) const;

    protected:
        virtual void execute(processor_t& processor) const = 0;
    };

    class assign_t : public instruction_t {
        std::string variable_;
        uint64_t value_;
    public:
        op_type_t op_type() const override
        {
            return op_type_t::assign;
        }
        void populate(std::istream& in) override
        {
            std::string dummy;
            in >> dummy >> value_;
        }
        std::string to_string() override
        {
            std::stringstream temp;
            temp << variable_ << " = " << value_;
            return temp.str();
        }

        assign_t(const std::string& src) : variable_(src), value_() {}
        virtual ~assign_t() {}

    protected:
        virtual void execute(processor_t& processor) const override;
    };

    class print_t : public instruction_t {
        std::string variable_;
    public:
        op_type_t op_type() const override
        {
            return op_type_t::print;
        }
        void populate(std::istream& in) override
        {
            in >> variable_;
        }
        std::string to_string() override
        {
            std::stringstream temp;
            temp << "print " << variable_;
            return temp.str();
        }

        print_t(const std::string& /*src*/) : variable_() {}
        virtual ~print_t() {}

    protected:
        virtual void execute(processor_t& processor) const override;
    };

    class lock_t : public instruction_t {
    public:
        op_type_t op_type() const override
        {
            return op_type_t::lock;
        }
        void populate(std::istream& /*in*/) override {}
        std::string to_string() override
        {
            return "lock";
        }

        lock_t(const std::string& /*src*/) {}
        virtual ~lock_t() {}

    protected:
        virtual void execute(processor_t& processor) const override;
    };

    class unlock_t : public instruction_t {
    public:
        op_type_t op_type() const override
        {
            return op_type_t::unlock;
        }
        void populate(std::istream& /*in*/) override {}
        std::string to_string() override
        {
            return "unlock";
        }

        unlock_t(const std::string& /*src*/) {}
        virtual ~unlock_t() {}

    protected:
        virtual void execute(processor_t& processor) const override;
    };

    class end_t : public instruction_t {
    public:
        op_type_t op_type() const override
        {
            return op_type_t::end;
        }
        void populate(std::istream& /*in*/) override {}
        std::string to_string() override
        {
            return "end";
        }

        end_t(const std::string& /*src*/) {}
        virtual ~end_t() {}

    protected:
        virtual void execute(processor_t& processor) const override;
    };


    class programm_t {
        status_t status_;
        std::vector<std::shared_ptr<instruction_t>> body_;
        std::vector<std::shared_ptr<instruction_t>>::iterator pc_;

    public:
        programm_t(): status_(status_t::running), body_(), pc_() {}

        status_t  status() const
        {
            return status_;
        }
        status_t& status()
        {
            return status_;
        }

        std::shared_ptr<instruction_t> instruction()
        {
            return *pc_;
        }
        void step_back()
        {
            --pc_;
        }
        void step_forward()
        {
            ++pc_;
        }
        time_t execute_step(processor_t& proc);

        void add_instruction(std::shared_ptr<instruction_t> instruction)
        {
            body_.push_back(instruction);
        }

        friend std::istream& operator>>(std::istream& in, programm_t& prog)
        {
            std::string line;

            while (std::getline(in, line) && line != "end") {
                std::stringstream sin(line);
                std::string op;
                sin >> op;
                auto instruction = instruction_t::parse(op);
                instruction->populate(sin);
                prog.add_instruction(instruction);
            }

            prog.add_instruction(std::make_shared<end_t>(""));
            prog.pc_ = prog.body_.begin();
            return in;
        }

        friend std::ostream& operator <<(std::ostream& out, const programm_t& prog)
        {
            for (auto it = prog.body_.begin(); it != prog.body_.end(); ++it) {
                out << (it == prog.pc_ ? "=>" : "  ") << (*it)->to_string() // @suppress("Method cannot be resolved")
                    << std::endl; // @suppress("Invalid overload")
            }

            return out;
        }
    };

    struct op_type_hash {
        std::hash<char> base_;
        size_t operator()(const op_type_t& op_type) const
        {
            return base_(static_cast<char>(op_type));
        }
    };

    class processor_t {
        typedef size_t proc_id_t;
        size_t n_proc_;
        std::unordered_map<proc_id_t, std::shared_ptr<programm_t>> programms_;
        std::unordered_map<char, uint64_t> variables_;
        std::unordered_map<op_type_t, time_t, op_type_hash> timings_;
        time_t quantum_;
        time_t slot_time_;
        // runtime
        std::string out_;
        proc_id_t current_;
        std::deque<proc_id_t> ready_;
        proc_id_t latch_;
        std::deque<proc_id_t> locked_;
    public:
        processor_t() : n_proc_(), programms_(), variables_(), timings_(), quantum_(), slot_time_(), out_(), current_(), ready_(),
            latch_(std::numeric_limits<proc_id_t>::max()), locked_() {}

        friend std::istream& operator>>(std::istream& in, processor_t& proc);
        friend std::ostream& operator<<(std::ostream& out, const processor_t& proc);

        void set(const std::string& variable, const uint64_t value)
        {
            variables_[variable[0]] = value;
        }
        void print(const std::string& variable);
        void lock();
        void unlock();
        void terminate()
        {
            programms_.find(current_)->second->status() = status_t::terminated;
        }

        time_t timings(op_type_t op_type)
        {
            return timings_[op_type];
        }

        void run();

#ifdef _DEBUG
        void dump(const std::string& comment);
#endif
    };

    class solution_t {
    public:
        solution_t() : n_cases_(std::numeric_limits<size_t>::max()), processor_() {}

        operator bool()
        {
            return n_cases_-- != 0;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        size_t n_cases_;
        std::unique_ptr<processor_t> processor_;
    };

    solution_t& solution_t::operator()()
    {
        processor_->run();
        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        sol.processor_.release();

        if (sol.n_cases_ == std::numeric_limits<size_t>::max()) {
            in >> sol.n_cases_;
        }

        if (in) {
            sol.processor_.reset(new processor_t);
            in >> *sol.processor_;
        }

        return in;
    }

    std::istream& operator>>(std::istream& in, processor_t& proc)
    {
        time_t assign_time(0), print_time(0), lock_time(0), unlock_time(0), end_time(0);
        in >> proc.n_proc_ >> assign_time >> print_time >> lock_time >> unlock_time >> end_time >> proc.quantum_;
        proc.timings_ = {{op_type_t::assign, assign_time}, {op_type_t::print, print_time}, {op_type_t::lock, lock_time}, {op_type_t::unlock, unlock_time}, {op_type_t::end, end_time}};
        in.ignore();

        for (size_t idx = 0; idx < proc.n_proc_; idx++) {
            auto prog = std::make_shared<programm_t>();
            in >> *prog;
            size_t pid = proc.programms_.size() + 1;
            proc.programms_.insert(std::make_pair(pid, prog));
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        out << *sol.processor_; // << std::endl;
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const processor_t& proc)
    {
        out << proc.out_;
        return out;
    }

    std::shared_ptr<instruction_t> instruction_t::parse(const std::string& src)
    {
        static std::unordered_map<std::string, op_type_t> code_map = {
            {"print", op_type_t::print},
            {"lock", op_type_t::lock},
            {"unlock", op_type_t::unlock},
            {"end", op_type_t::end}
        };
        auto code = code_map.find(src);

        if (code != code_map.end()) {
            switch (code->second) {
            case op_type_t::end:
                return std::make_shared<end_t>(src);

            case op_type_t::lock:
                return std::make_shared<lock_t>(src);

            case op_type_t::unlock:
                return std::make_shared<unlock_t>(src);

            case op_type_t::print:
                return std::make_shared<print_t>(src);

            default:
                break;
            }
        }

        return std::make_shared<assign_t>(src);
    }

    void processor_t::print(const  std::string& variable)
    {
        std::stringstream tmp;
        tmp << current_ << ": " << variables_[variable[0]] << std::endl;
        out_ += tmp.str();
    }

    void assign_t::execute(processor_t& proc) const
    {
#ifdef _DEBUG
        std::clog << variable_ << " = " << value_ << std::endl;
#endif
        proc.set(variable_, value_);
    }

    void print_t::execute(processor_t& proc) const
    {
#ifdef _DEBUG
        std::clog << "print " << variable_ << std::endl;
#endif
        proc.print(variable_);
    }

    void lock_t::execute(processor_t& proc) const
    {
#ifdef _DEBUG
        std::clog << "lock" << std::endl;
#endif
        proc.lock();
    }

    void unlock_t::execute(processor_t& proc) const
    {
#ifdef _DEBUG
        std::clog << "unlock" << std::endl;
#endif
        proc.unlock();
    }

    void end_t::execute(processor_t& proc) const
    {
#ifdef _DEBUG
        std::clog << "end" << std::endl;
#endif
        proc.terminate();
    }

    void processor_t::lock()
    {
        if (latch_ == std::numeric_limits<proc_id_t>::max()) {
            latch_ = current_;
        }

        if (latch_ != current_) {
            locked_.push_back(current_);
            programms_.find(current_)->second->status() = status_t::locked;
        }
    }

    void processor_t::unlock()
    {
        if (latch_ == current_) {
            if (not locked_.empty()) {
                auto unlock = locked_.front();
                auto prog = programms_.find(unlock)->second;
                prog->status() = status_t::running;
                ready_.push_front(unlock);
                locked_.pop_front();
            }

            latch_ = std::numeric_limits<proc_id_t>::max();
        }
    }

    void processor_t::run()
    {
        std::transform(programms_.begin(), programms_.end(), //
                       std::back_inserter(ready_), //
                       [](const std::pair<proc_id_t, std::shared_ptr<programm_t>>& prog) ->proc_id_t { return prog.first; });
        std::sort(ready_.begin(), ready_.end());

        while (not ready_.empty()) {
            current_ = ready_.front();
            ready_.pop_front();
            auto prog = programms_.find(current_)->second;
            slot_time_ = 0;

            while (prog->status() == status_t::running && slot_time_ < quantum_) {
                slot_time_ += prog->execute_step(*this);
            }

            if (prog->status() == status_t::running) {
                ready_.push_back(current_);
            }
        }
    }

    time_t programm_t::execute_step(processor_t& processor)
    {
        auto exec_time = instruction()->exec(processor);

        if (status_ == status_t::running) {
            ++pc_;
        }

        return exec_time;
    }

    time_t instruction_t::exec(processor_t& processor) const
    {
        execute(processor);
#ifdef _DEBUG
        processor.dump("after");
#endif
        return processor.timings(op_type());
    }

#ifdef _DEBUG
    std::ostream& operator<<(std::ostream& out, status_t status)
    {
        switch (status) {
        case status_t::locked:
            out << "locked";
            break;

        case status_t::running:
            out << "running";
            break;

        case status_t::terminated:
            out << "terminated";
            break;

        default:
            out << "UNDEFINED";
            break;
        }

        return out;
    }

    void processor_t::dump(const std::string& comment)
    {
        std::clog << "******* " << comment << " => current: " << current_ << "/"
                  << (latch_ == std::numeric_limits<proc_id_t>::max() ? 0 : latch_)
                  << "/" << programms_.find(current_)->second->status();
        std::clog << "; times: " << slot_time_ << "/"
                  << timings_.find(programms_.find(current_)->second->instruction()->op_type())->second << "/"
                  << quantum_ << "; Variables: [";
        std::ostream_iterator<std::string> val_out(std::clog, " ");
        std::transform(variables_.begin(), variables_.end(), val_out, [](const std::pair<char, uint64_t>& val) {
            std::stringstream val_str;
            val_str << "[" << val.first << "] = " << val.second;
            return val_str.str();
        });
        std::clog << "]; Ready: [";
        std::ostream_iterator<proc_id_t> piout(std::clog, " ");
        std::copy(ready_.begin(), ready_.end(), piout);
        std::clog << "]; Pending: [";
        std::copy(locked_.begin(), locked_.end(), piout);
        std::clog << "]" << std::endl;
        auto prog = programms_.find(current_)->second;
        std::clog << "Programm position:" << std::endl << *prog << std::endl;
        std::clog << "Output so far:" << std::endl << out_ << std::endl;
    }
#endif
}

void U210::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
