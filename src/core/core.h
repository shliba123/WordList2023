//
// Created by shliba on 2023/3/7.
//

#ifndef WORDLIST2023_CORE_H
#define WORDLIST2023_CORE_H


#include "graph.h"
#include "iostream"
#include "../control/util.h"

using std::cout;
using std::endl;

/**
 * 统计所有的单词链
 * @param words 单词数组
 * @param wordsLen 单词数组大小
 * @param result 结果数组，每一个元素是一个单词链
 * @return 单词链的个数
 */
int countChains(char *words[], int wordsLen, char *result[]);

/**
 * 以单词形式统计最长的单词链
 * @param words 单词数组
 * @param wordsLen 单词数组大小
 * @param result 结果数组
 * @param head 指定首字母，如果指定，则范围为 'a' ~ 'z'，若没有指定则为 0
 * @param tail 指定尾字母，如果指定，则范围为 'a' ~ 'z'，若没有指定则为 0
 * @param ban 指定禁用的首字母，如果指定，则范围为 'a' ~ 'z'，若没有指定则为 0
 * @param allowLoop 是否允许环
 * @return 最长的单词链的长度
 */
int getLongestWordChain(char *words[], int wordsLen, char *result[], char head, char tail, char ban, bool allowLoop);

/**
 * 以字符形式统计最长的单词链
 * @param words 单词数组
 * @param wordsLen 单词数组大小
 * @param result 结果数组
 * @param head 指定首字母，如果指定，则范围为 'a' ~ 'z'，若没有指定则为 0
 * @param tail 指定尾字母，如果指定，则范围为 'a' ~ 'z'，若没有指定则为 0
 * @param ban 指定禁用的首字母，如果指定，则范围为 'a' ~ 'z'，若没有指定则为 0
 * @param allowLoop 是否允许环
 * @return 最长的单词链的长度
 */
int getLongestCharChain(char *words[], int wordsLen, char *result[], char head, char tail, char ban, bool allowLoop);

namespace WordChain
{
    namespace Loop
    {
    /**
     * 解决函数的主方法
     */
    int mainSolution(char *words[], int wordsLen, char *result[],
                     char head, char tail, char ban);

    /**
     * dfs 的包装方法，用于对每个顶点进行 dfs
     * @param rawGraph 初始图
     * @param sccDistance 距离矩阵，记录着从两点间的最大距离（不考虑起点的自环）
     */
    void getSccDistance(Graph *rawGraph, int sccDistance[][MAX_VERTEX]);

    /**
     * dfs 本体函数
     * @param scc 连通分量
     * @param start 起始点
     * @param cur 当前点
     * @param step 已经走过的路径长度
     * @param edgeVisit dfs visit 辅助数组
     * @param vertexVisit 辅助数组，用于避免自环的重复计算
     * @param sccDistance 距离矩阵，记录着从两点间的最大距离
     */
    void dfsSccDistance(Graph *scc, int start, int cur, int step,
                        bool edgeVisit[], int vertexVisit[], int sccDistance[][MAX_VERTEX]);

    /**
     * dp 本体，需要先考虑 scc 内部 dfs 的成果和 hostGraph 的 topo dp
     * @param head
     * @param ban
     * @param rawGraph
     * @param sccDistance
     * @param dp
     * @param hostPreEdge hostPreEdge[v] = e 表示 v 这个节点的入边（路径入边）在 hostGraph 里的编号是 e
     * @param sccPreVertex 记录着在 scc 内部，dfs 结果的前一个`节点`
     * @return int 返回路径的最终节点的编号
     */
    int dpSolve(char head, char tail, Graph *rawGraph, int sccDistance[MAX_VERTEX][MAX_VERTEX],
                int dp[], Edge* hostPreEdge[], int sccPreVertex[]);
    /**
     * 用于产生答案，其中用到了 dp 跟踪数组和 scc 的 dfs
     * @param cur 当前节点
     * @param hostPreEdge
     * @param sccPreVertex
     * @param rawGraph
     * @param sccDistance
     * @param result
     * @return 单词链的中单词的个数
     */
    int restoreChain(int cur, Edge* hostPreEdge[], const int sccPreVertex[], Graph *rawGraph,
                     int sccDistance[][MAX_VERTEX], char *result[]);

    /**
     * 本质是根据约束条件再次进行一遍 dfs，直到找到满足条件的路径
     * @param cur
     * @param dest 路径的终点
     * @param remain 剩余的路径长度
     * @param subChain
     * @return 搜索是否成功
     */
    bool restoreSubChain(Graph *scc, int cur, int dest, int remain,
                         bool edgeVisit[], int vertexVisit[], list<char *> &subChain);
    }

    namespace NoLoop
    {
    int mainSolution(char *words[], int wordsLen, char *result[],
                     char head, char tail, char ban);
    int dpSolve(char head, char tail, Graph *rawGraph, int dp[], Edge* preEdge[]);
    int restoreChain(int cur, Edge* preEdge[], Graph *rawGraph, char *result[]);
    }
}

namespace CharChain
{
    namespace Loop
    {
    int mainSolution(char *words[], int wordsLen, char *result[],
                     char head, char tail, char ban);
    void getSccDistance(Graph *rawGraph, int sccDistance[][MAX_VERTEX]);
    void dfsSccDistance(Graph *scc, int start, int cur, int step,
                        bool edgeVisit[], int vertexVisit[], int sccDistance[][MAX_VERTEX]);
    int dpSolve(char head, char tail, Graph *rawGraph, int sccDistance[30][30],
                                 int dp[], Edge* hostPreEdge[], int sccPreVertex[]);
    int restoreChain(int cur, Edge* hostPreEdge[], const int sccPreVertex[], Graph *rawGraph,
                     int sccDistance[][MAX_VERTEX], char *result[]);
    bool restoreSubChain(Graph *scc, int cur, int dest, int remain,
                         bool edgeVisit[], int vertexVisit[], list<char *> &subChain);
    }
    namespace NoLoop
    {
    /**
     * 纠结允许有环时字母最长的单词链
     */
    int mainSolution(char *words[], int wordsLen, char *result[],
                     char head, char tail, char ban);
    /**
     * 相比于有环的时候必须考虑两个 pre，这次只有一个 pre
     * @param head
     * @param tail
     * @param ban
     * @param rawGraph
     * @param dp
     * @param preEdge
     * @return
     */
    int dpSolve(char head, char tail, Graph *rawGraph, int dp[], Edge* preEdge[]);
    int restoreChain(int cur, Edge* preEdge[], Graph *rawGraph, char *result[]);
    }
}

void dfsChain(int, Graph *, int length, char *result[], int &chainCount, bool visit[], vector<char *> &chain);

#endif // WORDLIST2023_CORE_H
