#pragma once

#include <exception>
#include <string>

class Error: public std::exception 
{
public:
    enum ErrorType {
        VERB_NOT_FOUND,
        EMPTY_SENTENCE,
        EMPTY_QUESTION_TAG,
        DICTIONARY_CAN_BE_OPEN
    };

    Error( ErrorType, std::string = "" ) throw();
    virtual ~Error() throw();
    virtual const char* what() const throw();
    ErrorType getType() const throw();

private:
    std::string _message;
    ErrorType   _type;
};