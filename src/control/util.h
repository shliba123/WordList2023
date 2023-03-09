//
// Created by thysrael on 23-3-8.
//

#ifndef WORDLIST2023_UTIL_H
#define WORDLIST2023_UTIL_H

#include <exception>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <iostream>
enum ErrorType
{
    FILE_NOT_FIND = 1,
    HAVE_LOOP_ERROR,
    MULTI_FILE_PATH,
    PARAMETER_NOT_EXISTS,
    NO_FILE_PATH,
    FILE_PATH_FORM_ERROR,
    NO_CHAR_ERROR,
    CHAR_FORM_ERROR,
    PARAMETER_FORM_ERROR,
    ALLOC_MEMORY_ERROR,
    MULTI_WORK_ERROR,
    NO_WORK_ERROR,
    FIRST_CHAR_DUPLICATE,
    FINAL_CHAR_DUPLICATE,
    ENABLE_LOOP_DUPLICATE,
    N_WORK_WITH_OTHER_PARAMETER,
    WORD_NOT_AVAILABLE,
    HAVE_LOOP
};

const static std::unordered_map<ErrorType, std::string> errorMap = {
    {MULTI_FILE_PATH, "指定了多个文件路径，请仅指定单一路径!"},
    {ALLOC_MEMORY_ERROR, "申请空间异常，请重新尝试!"},
    {PARAMETER_NOT_EXISTS, "参数不存在，请重新输入!"},
    {MULTI_WORK_ERROR, "指定了多个任务，请仅指定一个任务！"},
    {NO_CHAR_ERROR, "指定首尾字母时忘记字母参数!"},
    {CHAR_FORM_ERROR, "指定字母时格式不正确！只允许指定大小写字母！"},
    {FIRST_CHAR_DUPLICATE, "重复指定首字母！"},
    {ENABLE_LOOP_DUPLICATE, "重复指定有环参数!"},
    {NO_FILE_PATH, "参数中不存在文件路径!"},
    {N_WORK_WITH_OTHER_PARAMETER, "-n 参数不支持和其他参数共同使用!"},
    {NO_WORK_ERROR, "未指定任务，请指定一个任务!"},
    {WORD_NOT_AVAILABLE, "不存在符合要求的单词链"},
    {FILE_NOT_FIND, "输入文件没有找到"}
};

class MyException : std::runtime_error
{
public:
    ErrorType errorType;
    explicit MyException(ErrorType errorType);
    const char *what() const noexcept override;
};
#endif // WORDLIST2023_UTIL_H
