//
// Created by thysrael on 23-3-8.
//

#ifndef WORDLIST2023_IO_H
#define WORDLIST2023_IO_H

#include "parse.h"
#include "unordered_set"

/**
 * 从文件中读取单词
 * 实现中采用状态机思想，用两个状态确定是否要保存单词
 * 需要一个 wordSet 完成去重操作
 * @param inputFilePath 文件路径
 * @param words 最终的单词数组
 * @throw FILE_NOT_FIND 输入文件没有找到
 * @return 单词的个数
 */
int readWords(char *inputFilePath, char *words[]);

#endif // WORDLIST2023_IO_H
