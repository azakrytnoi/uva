#include "logger.h"

logger* logger::instance_(nullptr);

std::mutex logger::log_mutex_;

void logger::check()
{
    std::lock_guard<std::mutex> l(logger::log_mutex_);
    if (logger::instance_ == nullptr) {
        logger::instance_ = new logger;
    }
}

logger::logger()
{
    out_ = std::ofstream("test.log");
    sstream_ = new log_stream(*this);
}

int main()
{
    logger::log() << "simple test " << true;
}
