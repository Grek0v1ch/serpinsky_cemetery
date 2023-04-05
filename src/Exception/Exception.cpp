#include "Exception.h"

namespace Exception {
    Exception::Exception(const std::string& msg) noexcept : m_msg(msg) {}

    const char *Exception::what() const noexcept {
        return m_msg.c_str();
    }

    void Exception::addMsg(const std::string& newMsg) noexcept {
        m_msg = newMsg + m_msg;
    }
}