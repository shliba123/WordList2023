//
// Created by thysrael on 23-3-8.
//

#include <cstring>
#include "graph.h"
#include "../control/util.h"

Edge::Edge(int source, int target, int weight, int index, char *word) :
    source(source), target(target), weight(weight), index(index), word(word)
{
}
int Edge::getSource() const
{
    return source;
}
int Edge::getTarget() const
{
    return target;
}
int Edge::getWeight() const
{
    return weight;
}
char *Edge::getWord() const
{
    return word;
}
int Edge::getIndex() const
{
    return index;
}

Graph::Graph(char **words, int wordsLen)
{
    edgeNum = 0;
    for (int i = 0; i < wordsLen; ++i) {
        int len = (int)strlen(words[i]);
        if (words[i][0] == words[i][len - 1]) {
            addSelfLoop(words[i]);
        } else {
            addEdge(words[i]);
        }
    }
    memset(sccIndex, 255, sizeof(sccIndex));
}
Graph::~Graph()
{
    for (const auto &edges : vertexEdges)
    {
        for (const auto &edge : edges)
        {
            delete edge;
        }
    }
    for (const auto &loops : vertexSelfLoop)
    {
        for (const auto &loop : loops)
        {
            delete loop;
        }
    }
    for (const auto &scc : sccs)
    {
        delete scc;
    }
}
void Graph::addEdge(int source, int target, char *word)
{
    auto edge = new Edge(source, target, (int)strlen(word), edgeNum++, word);
    vertexEdges[source].push_back(edge);
    inDegree[target]++;
}
void Graph::addEdge(char *word)
{
    int wordLen = (int)strlen(word);
    int source = (int)word[0] - 'a';
    int target = (int)word[wordLen - 1] - 'a';
    auto edge = new Edge(source, target, wordLen, edgeNum++, word);
    vertexEdges[source].push_back(edge);
    inDegree[target]++;
}
void Graph::addSelfLoop(char *word)
{
    int wordLen = (int)strlen(word);
    int source = (int)word[0] - 'a';
    auto edge = new Edge(source, source, wordLen, edgeNum++, word);
    vertexSelfLoop[source].push_back(edge);
    // 相比于非自环边，自环边需要加入节点权重
    vertexWeight[source]++;
    vertexCharWeight[source] += wordLen;
}

void Graph::compressGraph()
{
    // 为 tarjan 算法准备
    bool visit[MAX_VERTEX] = {};
    bool inStack[MAX_VERTEX] = {};
    int low[MAX_VERTEX] = {0};
    int dfn[MAX_VERTEX] = {0};
    int stack[MAX_VERTEX] = {0};
    int top = 0;
    int dfnNum = 0;
    for (int v = 0; v < ALPHA_SIZE; v++)
    {
        if (!visit[v])
        {
            tarjan(v, visit, inStack, low, dfn, stack, top, dfnNum);
        }
    }

    // 新建 hostGraph
    hostGraph = new Graph({}, 0);
    // 为 hostGraph 和 scc 加边
    for (int v = 0; v < ALPHA_SIZE; v++)
    {
        for (auto& edge : vertexEdges[v])
        {
            int target = edge->getTarget();
            // 如果在同一个 scc 中，则给 scc 加边
            if (sccIndex[v] == sccIndex[target])
            {
                sccs[sccIndex[v]]->addEdge(edge->getWord());
            }
            // 如果不在同一个 scc 中，那么则给 hostGraph 加边
            else
            {
                hostGraph->addEdge(sccIndex[v], sccIndex[target], edge->getWord());
            }
        }
    }
    // 一模一样的逻辑，针对自环，只不过自环只会出现在 scc 中
    for (int v = 0; v < ALPHA_SIZE; v++)
    {
        for (auto& edge : vertexSelfLoop[v])
        {
            sccs[sccIndex[v]]->addSelfLoop(edge->getWord());
        }
    }

#ifdef DEBUG
    cout << "scc num: " << sccs.size() << endl;
    cout << "scc index: " << endl;
    for (int i = 0; i < ALPHA_SIZE; i++)
        printf("%4c", i + 'a');
    cout << endl;
    for (int i = 0; i < ALPHA_SIZE; i++)
        printf("%4d", sccIndex[i]);
    cout << endl;
#endif
}

void Graph::tarjan(int v, bool *visit, bool *inStack, int *low, int *dfn, int *stack,
                   int &top, int &dfnNum)
{
    low[v] = dfn[v] = ++dfnNum;
    stack[++top] = v;

    visit[v] = inStack[v] = true;
    for (auto& edge : vertexEdges[v])
    {
        int target = edge->getTarget();
        if (!visit[target])
        {
            tarjan(target, visit, inStack, low, dfn, stack, top, dfnNum);
            low[v] = std::min(low[v], low[target]);
        }
        else if (inStack[target])
        {
            low[v] = std::min(low[v], low[target]);
        }
    }

    int topVertex;
    // 如果当前节点 x 是强连通分量的根
    if (low[v] == dfn[v])
    {
        // 这里进行弹栈
        do {
            topVertex = stack[top--];
            inStack[topVertex] = false;
            sccIndex[topVertex] = (int) sccs.size();
        } while (low[topVertex] != dfn[topVertex]);
        // 这里为刚才弹栈的点新建一个图
        auto *scc = new Graph({}, 0);
        sccs.push_back(scc);
    }
}
void Graph::topoSort(int vertexNum, int *ans)
{
    int degree[ALPHA_SIZE];
    int head = 0, tail = 0;
    // 复制入度，因为之后会对于 degree 进行操作
    for (int i = 0; i < vertexNum; ++i)
    {
        degree[i] = inDegree[i];
    }
    // 遍历所有的节点，应该是 26 个
    for (int i = 0; i < vertexNum; ++i)
    {
        // 如果入度为 0，就入队，这很 topoSort 正确
        if (!degree[i])
        {
            ans[tail++] = i;
        }
    }

    // 应该是说，从 ans 一开始遍历
    while (head < tail)
    {
        int pt = ans[head++];
        // 遍历以 pt 为节点的所有边集
        for (auto &edge : vertexEdges[pt])
        {
            int target = edge->getTarget();
            degree[target]--;
            if (!degree[target])
            {
                ans[tail++] = target;
            }
        }
    }

    // 如果最后 ans 的排序与节点数不等，那么就说明是有环的（无法拓扑）
    if (tail != vertexNum)
    {
        throw MyException(HAVE_LOOP);
    }
}

void Graph::delSingleEdges()
{
    // inDegree 和 outDegree 数组，没有使用自带的 inDegree 是因为删边会导致 inDegree 更改
    int ind[MAX_VERTEX] = {}, oud[MAX_VERTEX] = {};
    // 更新获得所有的出入度
    for (int i = 0; i < ALPHA_SIZE; i++)
    {
        for (auto& edge : vertexEdges[i])
        {
            int source = edge->getSource();
            int target = edge->getTarget();
            ind[target]++, oud[source]++;
        }
    }

    // 以下操作是因为单词链必须具有两个以上的单词，这些边显然是只有一条边的
    // 对于非自环边，孤立树边：边的起点、终点均没有自环，起点的入度为 0，终点的出度为 0
    for (int i = 0; i < ALPHA_SIZE; i++)
    {
        auto begin = vertexEdges[i].begin();
        auto end = vertexEdges[i].end();
        for (auto iter = begin; iter != end;)
        {
            int source = (*iter)->getSource(), target = (*iter)->getTarget();
            int sourceWeight = vertexWeight[source], targetWeight = vertexWeight[target];
            if (!ind[source] && !oud[target] && !sourceWeight && !targetWeight)
            {
                iter = vertexEdges[i].erase(iter);
            }
            else
            {
                iter++;
            }
        }
    }

    // for a self-loop-edge, we delete it when ind[x] == 0 && oud[x] == 0 && point_weigh[x] == 1
    // 孤立自环边：某个点只有一个自环，且该点入度、出度为 0
    for (int i = 0; i < ALPHA_SIZE; i++)
    {
        auto begin = vertexSelfLoop[i].begin();
        auto end = vertexSelfLoop[i].end();
        for (auto iter = begin; iter != end;)
        {
            int source = (*iter)->getSource();
            int sourceWeight = vertexWeight[source];
            if (!ind[source] && !oud[source] && sourceWeight == 1)
            {
                iter = vertexSelfLoop->erase(iter);
            }
            else
            {
                iter++;
            }
        }
    }
}

Graph *Graph::getHostGraph() const
{
    return hostGraph;
}
const vector<Graph *> &Graph::getSccs() const
{
    return sccs;
}

int Graph::getSccsNum() const
{
    return (int) sccs.size();
}
const int *Graph::getSccIndex() const
{
    return sccIndex;
}
const list<Edge *> *Graph::getVertexEdges() const
{
    return vertexEdges;
}
int Graph::getVertexWeight(int v) const
{
    return vertexWeight[v];
}
int Graph::getVertexCharWeight(int v) const
{
    return vertexCharWeight[v];
}
const int *Graph::getVertexWeights() const
{
    return vertexWeight;
}
const int *Graph::getVertexCharWeights() const
{
    return vertexCharWeight;
}
const list<Edge *> *Graph::getVertexSelfLoop() const
{
    return vertexSelfLoop;
}
