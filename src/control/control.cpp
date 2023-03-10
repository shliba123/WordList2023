//
// Created by thysrael on 23-3-8.
//
#include "iostream"
#include "control.h"
#include "io.h"
#include "../core/core.h"

char* words[MAX_WORD];
char* result[MAX_WORD];

void control(int argc, char *argv[])
{
    try
    {
        // 解析参数
        auto ans = parseParam(argc, argv);
        int problemType = get<0>(ans);
        char head = get<1>(ans);
        char tail = get<2>(ans);
        char ban = get<3>(ans);
        bool allowLoop = get<4>(ans);
        char *path = get<5>(ans);

        // 读取参数
        int wordsLen = readWords(path, words);

        int resultLen = 0;
        int chainCnt = 0;
        // 选择问题
        switch (problemType)
        {
        case COUNT_CHAINS:
            break;
        case MAX_CHAIN_WORD:
            resultLen = getLongestWordChain(words, wordsLen, result, head, tail, ban, allowLoop);
            break;
        case MAX_CHAIN_CHAR:
            resultLen = getLongestCharChain(words, wordsLen, result, head, tail, ban, allowLoop);
            break;
        default:
            break;
        }

        for (int i = 0; i < resultLen; i++)
        {
            cout << result[i] << endl;
        }
    }
    catch (MyException &e)
    {
        cerr << e.what();
    }
}