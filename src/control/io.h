//
// Created by thysrael on 23-3-8.
//

#ifndef WORDLIST2023_IO_H
#define WORDLIST2023_IO_H

#include "parse.h"
#include "unordered_set"
#include "cstdio"

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

/**
 * 输出结果，只有非 CHAIN_CNT 的任务类型需要输出文件
 * @param exePath 程序的路径，用于确定输出文件的路径
 * @param chainCnt 用于 CHAIN_CNT 题目，需要输出链的总数
 * @param result 结果数组
 * @param resultLen 结果数组的长度
 * @param problemType 任务类型
 */
void output(char *exePath, int chainCnt, char *result[], int resultLen, int problemType);

#endif // WORDLIST2023_IO_H
