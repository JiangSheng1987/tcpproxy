
#ifndef __function_failed_exception_h__
#define __function_failed_exception_h__

#include <exception>
#include <string>

namespace tcpproxy {

class FunctionFailedException : public std::exception
{
public:
    FunctionFailedException(const std::string& functionName, int exitCode)
    {
        m_msg = functionName + " failed. Exit code: " + std::to_string(exitCode);
    }

    const char* what() const noexcept
    {
        return m_msg.c_str();
    }

private:
    std::string m_msg;
};

} // namespace tcpproxy

#endif
