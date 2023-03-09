//
// Created by shliba on 2023/3/7.
//

#include "core.h"
#include "graph.h"

int getLongestWordChain(char *words[], int wordsLen, char *result[],
                        char head, char tail, char ban, bool allowLoop)
{
    // 分成两种形式求解
    if (allowLoop)
    {
        return getLongestWordChainWithLoop(words, wordsLen, result, head, tail, ban);
    }
    else
    {
        return getLongestWordChainWithoutLoop(words, wordsLen, result, head, tail, ban);
    }
}

int getLongestWordChainWithLoop(char *words[], int wordsLen, char *result[],
                                char head, char tail, char ban)
{
    auto rawGraph = new Graph(words, wordsLen);

}

int getLongestWordChainWithoutLoop(char *words[], int wordsLen, char *result[],
                                   char head, char tail, char ban)
{

}
