//
// Created by thysrael on 23-3-8.
//

#ifndef WORDLIST2023_PARSE_H
#define WORDLIST2023_PARSE_H

#include <tuple>
#include <cstring>
#include <exception>
#include <string>
#include <stdexcept>
#include "util.h"

#define WRONG_PROBLEM 0
#define COUNT_CHAINS 1
#define MAX_CHAIN_WORD 2
#define MAX_CHAIN_CHAR 3

/**
 * 解析参数
 * @param argc 参数个数
 * @param argv 参数内容
 * @return tuple，各个分量如下如是：
 *          0：问题类型
 *          1：head：指定起始字符，如果没有设置，则为 0，否则为 'a'-'z'
 *          2：tail: 指定终止字符，如果没有设置，则为0，否则为 'a'-'z'
 *          3：ban：指定禁止字符，如果没有设置，则为 0，否则为 'a'-'z'
 *          4：allowLoop：允许环则为 true，否则为 false
 *          5：filePath：输入文件路径
 */
std::tuple<int, int, int, int, bool, char*> parseParam(int argc, char *argv[]);

/**
 * 判断输入的字符串是否是文件路径
 * @param str 输入字符串
 * @return 是则为 true
 */
static bool inline isFilePath(char *str);

/**
 * 判断输入的字符串是不是一个字母
 * @param str 输入字符串
 * @return 是则为 true
 */
static bool inline isParamAlpha(char *str);

#endif // WORDLIST2023_PARSE_H
