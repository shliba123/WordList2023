//
// Created by thysrael on 23-3-8.
//

#include <cstring>
#include "graph.h"

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
void Graph::addSelfLoop(int source, char *word)
{
    auto edge = new Edge(source, source, (int)strlen(word), edgeNum++, word);
    vertexSelfLoop[source].push_back(edge);
}
void Graph::addSelfLoop(char *word)
{
    int wordLen = (int)strlen(word);
    int source = (int)word[0] - 'a';
    auto edge = new Edge(source, source, wordLen, edgeNum++, word);
    vertexSelfLoop[source].push_back(edge);
}

void Graph::compressGraph()
{
    bool visit[ALPHA_SIZE] = {};
    bool inStack[ALPHA_SIZE] = {};
    int low[ALPHA_SIZE] = {};
    int dfn[ALPHA_SIZE] = {};
    int stack[ALPHA_SIZE] = {};
    int color[ALPHA_SIZE] = {};
    int top;
    int dfnNum = 0, sccNum = 0;
    for (int v = 0; v < ALPHA_SIZE; v++) {
        if (!visit[v]) {
            tarjan(v, visit, inStack, low, dfn, stack, color, top, dfnNum, sccNum);
        }
    }
}

void Graph::tarjan(int v, bool visit[], bool inStack[], int low[], int dfn[], int stack[], int color[],
                   int &top, int &dfnNum, int &sccNum)
{
    low[v] = dfn[v] = ++dfnNum;
    stack[++top] = v;
    visit[v] = inStack[v] = true;
    for (auto &edge : vertexEdges[v]) {
        int target = edge->getTarget();
        if (!visit[target]) {
            tarjan(target, visit, inStack, low, dfn, stack, color, top, dfnNum, sccNum);
            low[v] = std::min(low[v], low[target]);
        } else if (inStack[target]) {
            low[v] = std::min(low[v], low[target]);
        }
    }

    int topPoint;
    // 如果当前节点 x 是强连通分量的根
    if (low[v] == dfn[v]) {
        do {
            topPoint = stack[top--];
            inStack[topPoint] = false;
            color[topPoint] = sccNum;
        } while (low[topPoint] != dfn[topPoint]);
        sccNum++;
    }
}

const list<Edge *> *Graph::getVertexEdges() const
{
    return vertexEdges;
}
