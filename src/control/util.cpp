//
// Created by thysrael on 23-3-8.
//

#include "util.h"

MyException::MyException(ErrorType errorType) :
    runtime_error(errorMap.find(errorType)->second), errorType(errorType)
{
}

const char *MyException::what() const noexcept
{
    return errorMap.find(errorType)->second.c_str();
}