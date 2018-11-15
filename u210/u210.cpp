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

void __cdecl invoke() {
    U210 instance;
    instance();
}

namespace {

enum class status_t
    : int
{
    running, ready, waiting, terminated
};

enum class id_t
    : int
{
    assignment, output, lock, unlock, end
};

class system_t;
class operator_t {
public:
    virtual ~operator_t() {
    }

    virtual void operator()(system_t& sys) = 0;
    virtual id_t id() const = 0;
};

class assignment_t: public operator_t {
    char variable_;
    uint16_t value_;

public:
    assignment_t(const std::string& line) :
        variable_(), value_() {
        std::stringstream iss(line);
        std::istream_iterator<std::string> begin(iss), end;
        const std::vector<std::string> tokens (begin, end);
        variable_ = tokens[0][0];
        value_ = std::atoi(tokens[2].c_str());
    }

    virtual void operator ()(system_t& sys) override;
    virtual id_t id() const override {
        return id_t::assignment;
    }
};

class output_t: public operator_t {
    char variable_;

public:
    output_t(const std::string& line) :
        variable_() {
        std::stringstream iss(line);
        std::istream_iterator<std::string> begin(iss), end;
        std::vector<std::string> tokens (begin, end);
        variable_ = tokens[0][0];
    }

    virtual void operator()(system_t& sys) override;
    virtual id_t id() const override {
        return id_t::output;
    }
};

class lock_t: public operator_t {
public:
    lock_t(const std::string& line) {
    }

    virtual void operator()(system_t& sys) override;
    virtual id_t id() const override {
        return id_t::lock;
    }
};

class end_t: public operator_t {
public:
    end_t(const std::string& line) {
    }

    virtual void operator()(system_t& sys) override;
    virtual id_t id() const override {
        return id_t::end;
    }
};

class unlock_t: public operator_t {
public:
    unlock_t(const std::string& line) {
    }

    virtual void operator()(system_t& sys) override;
    virtual id_t id() const override {
        return id_t::unlock;
    }
};

class programm_t {
    status_t status_;
    uint16_t id_;
    std::vector<std::shared_ptr<operator_t>> body_;

public:
    programm_t(uint16_t id) : status_(status_t::ready), id_(id), body_() {}

    void addOperator (const std::string& line);
    uint16_t id() const {
        return id_;
    }
    status_t status() const {
        return status_;
    }

    void start() {
        status_ = status_t::running;
    }
    void suspend() {
        status_ = status_t::ready;
    }
    void terminate() {
        status_ = status_t::terminated;
    }
};

class system_t {
    std::unordered_map<char, int16_t> variables_;
    std::unordered_map<uint16_t, std::shared_ptr<programm_t>> programms_;
    std::unordered_map<id_t, uint16_t> timings_;
    uint16_t time_, quantum_;
    uint16_t current_;
    std::deque<uint16_t> locks_;

public:
    system_t() :
        variables_(), programms_(), timings_(), time_(), quantum_(), current_(), locks_() {
    }

    std::vector<std::shared_ptr<programm_t>>& programms() {
        return programms_;
    }

    void reset(size_t N, uint16_t assign_time, uint16_t print_time,
               uint16_t lock_time, uint16_t unlock_time, uint16_t end_time, uint16_t quant) {
        time_ = 0;
        quantum_ = quant;
        timings_ = { {id_t::assignment, assign_time}, {id_t::output, print_time}, {id_t::lock, lock_time}, {id_t::unlock, unlock_time}, {id_t::end, end_time}};
        programms_.clear();
        programms_.reserve(N);
        locks_.clear();
    }

    void setVariable(char name, uint16_t val) {
        variables_[name] = val;
    }

    uint16_t getVariable(char name) {
        return variables_[name];
    }

    void lockProgram() {
        locks_.push_back(current_);
    }

    void unlockProgram() {
        locks_.erase(std::find(locks_.begin(), locks_.end(), current_));
    }

    void terminate() {
//		current_->terminate();
    }

    void run();

};

class solution {
public:
    solution() :
        system_() {
    }

    friend std::istream& operator >>(std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const {
        return true;
    }
    solution& operator()();

private:
    system_t system_;
};

std::istream& operator >>(std::istream& in, solution& sol) {
    size_t N;
    uint16_t assign_time, print_time, lock_time, unlock_time, end_time;
    uint16_t quantum_time;
    in >> N >> assign_time >> print_time >> lock_time >> unlock_time >> end_time >> quantum_time;
    sol.system_.reset(N, assign_time, print_time, lock_time, unlock_time, end_time, quantum_time);
    std::string line;
    std::getline(in, line);
    while(N--) {
        auto prog = std::make_shared<programm_t>(sol.system_.programms().size() + 1);
        while (std::getline(in, line) && line != "end") {
            prog->addOperator(line);
        }
        prog->addOperator("end");
        sol.system_.programms_[prog->id()] = prog;
    }
    return in;
}

std::ostream& operator <<(std::ostream& out, const solution& sol) {
    return out;
}

solution& solution::operator()() {
    system_.run();
    return *this;
}

void system_t::run() {
    current_ = std::min_element(programms_.begin(), programms_.end(),
    [](std::shared_ptr<programm_t>& p1, std::shared_ptr<programm_t>& p2) {
        return p1->id() < p2->id();
    } )->first;

}

void programm_t::addOperator(const std::string& line) {
    if (line == "lock") {
        body_.push_back(std::make_shared<lock_t>(line));
    } else if (line == "unlock") {
        body_.push_back(std::make_shared<unlock_t>(line));
    } else if (line == "end") {
        body_.push_back(std::make_shared<end_t>(line));
    } else if (line.substr(line.find(' ')) == "print") {
        body_.push_back(std::make_shared<output_t>(line));
    } else {
        body_.push_back(std::make_shared<assignment_t>(line));
    }
}

void assignment_t::operator ()(system_t& sys) {
    sys.setVariable (variable_, value_);
}

void output_t::operator ()(system_t& sys) {
    std::cout << sys.getVariable(variable_);
}

void lock_t::operator ()(system_t& sys) {
    sys.lockProgram();
}

void unlock_t::operator ()(system_t& sys) {
    sys.unlockProgram();
}

void end_t::operator ()(system_t& sys) {
    sys.terminate();
}
}

void U210::operator()() const {
    size_t N;
    std::cin >> N;
    solution sol;
    while (N-- != 0 && std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
