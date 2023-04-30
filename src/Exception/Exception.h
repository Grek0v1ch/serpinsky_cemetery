#pragma once

#include <exception>
#include <string>

namespace Exception {
    class Exception : public std::exception {
    private:
        std::string _msg;

    public:
        explicit Exception(const std::string& msg) noexcept;
        Exception(const Exception&) noexcept = default;
        ~Exception() override = default;

    public:
        virtual const char* what() const noexcept override;
        void addMsg(const std::string& newMsg) noexcept;
    };
}