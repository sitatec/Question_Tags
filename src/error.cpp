#include "include/error.hpp"

/**
 * @brief Error class, inherit `std::exception`
 * 
 * @param type Type of error message
 * @param message Error message
 */
Error::Error( ErrorType type, std::string message ) throw()
{
    _type    = type;
    _message = message;
}



Error::~Error() throw()
{}


/**
 * @brief Get the error type
 * 
 * @return Error::ErrorType 
 */
Error::ErrorType Error::getType() const throw()
{
    return _type;
}


/**
 * @brief Return the error message
 * 
 * @return const char* 
 */
const char* Error::what() const throw()
{
    return _message.c_str();
}
