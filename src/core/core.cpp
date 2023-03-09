//
// Created by shliba on 2023/3/7.
//

#include <string>
#include "cstring"
#include "core.h"
void saveChain(int &chainCount, char *result[], vector<char *> &chain)
{
    auto thisChain = std::string("");
    for (const auto &word : chain) {
        thisChain += word;
        thisChain += " ";
    }
    auto newResult = (char *)malloc(thisChain.size());
    for (int i = 0; i < thisChain.size() - 1; ++i) {
        newResult[i] = thisChain[i];
    }
    newResult[thisChain.size() - 1] = '\0';
    result[chainCount++] = newResult;
}

void dfsChain(int first, Graph *graph, int length, char *result[], int &chainCount, bool visit[], vector<char *> &chain)
{
    for (const auto &current : graph->getVertexEdges()[first]) {
        auto target = current->getTarget();
        auto currentIndex = current->getIndex();
        if (!visit[currentIndex]) {
            visit[currentIndex] = true;
            chain.push_back(current->getWord());
            if (length > 0) {
                saveChain(chainCount, result, chain);
            }
            dfsChain(target, graph, length + 1, result, chainCount, visit, chain);
            chain.pop_back();
            visit[currentIndex] = false;
        }
    }
}

int countChains(char *words[], int wordsLen, char *result[])
{
    auto rawGraph = new Graph(words, wordsLen);
    bool visit[10010] = {};
    int chainCount = 0;
    vector<char *> chain;
    for (int i = 0; i < 26; ++i) {
        dfsChain(i, rawGraph, 0, result, chainCount, visit, chain);
    }
    return chainCount;
}

int getLongestWordChain(char *words[], int wordsLen, char *result[],
                        char head, char tail, char ban, bool allowLoop)
{
    // 分成两种形式求解
    if (allowLoop) {
        return getLongestWordChainWithLoop(words, wordsLen, result, head, tail, ban);
    } else {
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
