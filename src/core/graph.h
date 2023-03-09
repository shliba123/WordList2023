//
// Created by thysrael on 23-3-8.
//

#ifndef WORDLIST2023_GRAPH_H
#define WORDLIST2023_GRAPH_H

#include "list"
#include "vector"
#include "algorithm"

using std::list;
using std::vector;

const int MAX_VERTEX = 30;
const int ALPHA_SIZE = 26;
class Edge
{
private:
    int source, target;
    int weight;
    char *word;
    int index;

public:
    /**
     * 之所以采用不直观的方法，是因为坍缩后的图的节点是 scc 而非字母，所以需要额外指定
     * @param source
     * @param target
     * @param weight
     * @param index 编号，主要方便 dfs
     * @param word
     */
    Edge(int source, int target, int weight, int index, char *word);
    int getSource() const;
    int getTarget() const;
    int getWeight() const;
    char *getWord() const;
    int getIndex() const;
};

class Graph
{
private:
    // 用来给 edge 当计数器
    int edgeNum;
    // 记录每一个点的所有出边，不包括自环
    list<Edge *> vertexEdges[MAX_VERTEX];
    // 记录每一个点的所有自环
    list<Edge *> vertexSelfLoop[MAX_VERTEX];
    // 记录每一个点的出度
    int inDegree[MAX_VERTEX] = {};
    // 记录每一个点的 word 权重
    int vertexWeight[MAX_VERTEX] = {};
    // 记录每一点的 char 权重
    int vertexCharWeight[MAX_VERTEX] = {};
    // 指示该图属于哪个 scc，如果为 -1 说明为主图
    int sccColor = -1;
    // 该图的 scc 集合
    vector<Graph *> sccs;
    // 主图，节点为 scc
    Graph *hostGraph = nullptr;

public:
    Graph(char *words[], int wordsLen);
    void addEdge(int source, int target, char *word);
    void addEdge(char *word);
    void addSelfLoop(int source, char *word);
    void addSelfLoop(char *word);
    /**
     * 对该图进行 tarjan scc 求解
     * 更新 sccs 和 hostGraph
     */
    void compressGraph();
    /**
     * 进行 tarjan 算法
     * @param v
     */
    void tarjan(int v, bool visit[], bool inStack[], int low[], int dfn[], int stack[], int color[],
                int &top, int &dfnNum, int &sccNum);
    const list<Edge *> *getVertexEdges() const;
};

#endif // WORDLIST2023_GRAPH_H
