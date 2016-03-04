#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10757.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <memory>
#include <map>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10757 instance;
    instance();
}

namespace {
enum class datatype : char {
    I = 'I', S = 'S'
};

class table {
    std::map<std::string, datatype> meta_;
    std::vector<std::map<std::string, std::string>> data_;
    std::string name_;

public:
    table() : meta_(), data_(), name_() {}

    const std::string& name() const
    {
        return name_;
    }

    const std::map<std::string, datatype>& meta() const
    {
        return meta_;
    }

    friend
    std::istream& operator >> (std::istream& in, table& t)
    {
        in >> t.name_;
        int nColumns, nRows;
        in >> nColumns >> nRows;
        std::vector<std::string> columns;
        columns.reserve(nColumns);
        while(nColumns--) {
            std::string name;
            std::string dt;
            in >> name >> dt;
            t.meta_[name] = datatype(dt[0]);
            columns.push_back(name);
        }
        t.data_.reserve(nRows);
        std::generate_n(std::back_inserter(t.data_), nRows, [&]() -> std::map<std::string, std::string> {
            std::map<std::string,std::string> row;
            std::for_each(columns.begin(), columns.end(), [&](auto name)
            {
                std::string value;
                in >> value;
                row.insert(std::make_pair(name, value));
            });
            return row;
        });
        return in;
    }
};

class database {
    std::map<std::string, std::string> columns_;
    std::map<std::string, std::shared_ptr<table>> tables_;

public:
    database() : columns_(), tables_() {}
    friend
    std::istream& operator >> (std::istream& in, database& d)
    {
        int nTables;
        std::cin >> nTables;
        while (nTables--) {
            auto t = std::make_shared<table>();
            std::cin >> *t;
            d.tables_[t->name()] = t;
            std::for_each(t->meta().begin(), t->meta().end(), [&](auto col) {
                d.columns_[col.first] = t->name();
            });
        }
        return in;
    }
};

class query {
    std::vector<std::string> selector_;
    std::vector<std::map<std::string, std::string>> resultset_;
    std::vector<std::pair<std::string, bool>> sort_;
    const database& db_;

public:
    query(const database& db) : selector_(), resultset_(), sort_(), db_(db) {}

    query& execute();

    friend
    std::istream& operator >> (std::istream& in, query& q)
    {
        std::string strq;
        {
            std::string line;
            while (std::getline(in, line)) {
                strq.append (line).append(" ");
            }
        }
        q.parse(strq);
        return in;
    }

    friend
    std::ostream& operator << (std::ostream& out, const query& q) {
        std::for_each(q.resultset_.begin(), q.resultset_.end(), [&](auto row) {
            std::for_each(q.selector_.begin(), q.selector_.end(), [&](auto col) {
                out << row[col] << " ";
            });
            std::cout << std::endl;
        });
        return out;
    }

    private:
    void parse(const std::string& qry);
};

void query::parse(const std::string& qry) {
    std::clog << qry << std::endl;
}

query& query::execute() { return *this;}

}

void U10757::operator()()
{
    int N;
    std::cin >> N;
    while (N--) {
        database db;
        std::cin >> db;
        query q(db);
        std::cin >> q;
        std::cout << q.execute() << std::endl;
    }
}
