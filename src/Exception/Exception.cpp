#include "Exception.h"

namespace Exception {
    Exception::Exception(const std::string& msg) noexcept : _msg(msg) {}

    const char *Exception::what() const noexcept {
        return _msg.c_str();
    }

    void Exception::addMsg(const std::string& newMsg) noexcept {
        _msg = newMsg + _msg;
    }
}