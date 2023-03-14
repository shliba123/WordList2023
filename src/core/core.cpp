//
// Created by shliba on 2023/3/7.
//

#include <cstring>
#include "core.h"
#include "graph.h"

/**
 * 这是在还原 scc 内路径时用到的临时数组，用与记录可能的路径。
 * 因为数组过大，所以开成了全局数组
 */
char *wordList[MAX_EDGE];
/**
 * 与 `wordList` 搭配使用，指示 `wordList` 的大小
 */
int wordListLen;

int getLongestWordChain(char *words[], int wordsLen, char *result[],
                        char head, char tail, char ban, bool allowLoop)
{
    // 分成两种形式求解
    if (allowLoop)
    {
        return WordChain::Loop::mainSolution(words, wordsLen, result, head, tail, ban);
    }
    else
    {
        return WordChain::NoLoop::mainSolution(words, wordsLen, result, head, tail, ban);
    }
}

int getLongestCharChain(char *words[], int wordsLen, char *result[],
                        char head, char tail, char ban, bool allowLoop)
{
    if (allowLoop)
    {
        return CharChain::Loop::mainSolution(words, wordsLen, result, head, tail, ban);
    }
    else
    {
        return CharChain::NoLoop::mainSolution(words, wordsLen, result, head, tail, ban);
    }
}

int WordChain::Loop::mainSolution(char *words[], int wordsLen, char *result[],
                                  char head, char tail, char ban)
{
    auto rawGraph = new Graph(words, wordsLen);
    // 删去 ban 的字符
    rawGraph->delBannedEdges(ban);
    // 进行 scc 压缩
    rawGraph->compressGraph();

    // 利用 dfs 去求解每一个 scc 中的距离
    // sccDistance[i][j] 表示由 i -> j 的最大距离，其中不包括 i 上的自环，如果 i, j 位于同一个 scc，如果不再，则为负数
    int sccDistance[MAX_VERTEX][MAX_VERTEX];
    memset(sccDistance, 255, sizeof(sccDistance));
    getSccDistance(rawGraph, sccDistance);

    // 利用 dpSolve 去求解 hostGraph 中的距离
    // dpSolve[i] 表示以 i 为结尾的单词链的最大长度
    int dp[MAX_VERTEX];
    memset(dp, 255, sizeof(int) * MAX_VERTEX);
    // 2 个 pre 数组用于还原路径，至于为啥只有 host 是记录边的，而 scc 内是记录点的，是因为 scc 内用 dfs 没法记录边
    // 在 scc 内还原路径时还需要重新 dfs
    int sccPreVertex[MAX_VERTEX];
    memset(sccPreVertex, 255, sizeof(int) * MAX_VERTEX);
    Edge *hostPreEdge[MAX_VERTEX] = {nullptr};
    // dest 是最终路径终点的编号
    int dest = dpSolve(head, tail, rawGraph, sccDistance, dp, hostPreEdge, sccPreVertex);

    // 根据 dest 和两个 pre 数组还原路径
    int resultLen = restoreChain(dest, hostPreEdge, sccPreVertex, rawGraph, sccDistance, result);
    delete rawGraph;
    return resultLen;
}

void WordChain::Loop::getSccDistance(Graph *rawGraph, int sccDistance[][MAX_VERTEX])
{
    // 显然每个点到自己的距离都是 0，因为本身的自环是不计算的
    for (int i = 0; i < ALPHA_SIZE; i++)
    {
        sccDistance[i][i] = 0;
    }

    int sccsNum = rawGraph->getSccsNum();
    // 遍历每一种颜色
    for (int i = 0; i < sccsNum; i++)
    {
        // 遍历每一个节点
        for (int j = 0; j < ALPHA_SIZE; j++)
        {
            // 如果该节点属于某个颜色
            if (rawGraph->getSccIndex()[j] == i)
            {
                bool *edgeVisit = new bool[MAX_EDGE];
                memset(edgeVisit, 0, sizeof(bool) * MAX_EDGE);
                int vertexVisit[MAX_VERTEX] = {0};
                // 进行 dfs
                dfsSccDistance(rawGraph->getSccs()[i], j, j, 0, edgeVisit, vertexVisit, sccDistance);
                delete[] edgeVisit;
            }
        }
    }
    // 打印 sccDistance 表格
#ifdef DEBUG
    printf("    ");
    for (int i = 0; i < ALPHA_SIZE; i++)
    {
        printf("%4c", i + 'a');
    }
    cout << endl;
    for (int i = 0; i < ALPHA_SIZE; i++)
    {
        printf("%4c", i + 'a');
        for (int j = 0; j < ALPHA_SIZE; j++)
        {
            printf("%4d", sccDistance[i][j]);
        }
        cout << endl;
    }
#endif
}

void WordChain::Loop::dfsSccDistance(Graph *scc, int start, int cur, int step,
                                     bool edgeVisit[], int vertexVisit[], int sccDistance[][MAX_VERTEX])
{
    vertexVisit[cur]++;
    // 遍历当前节点的每一条非自环边
    for (auto &edge : scc->getVertexEdges()[cur])
    {
        int target = edge->getTarget();
        if (!edgeVisit[edge->getIndex()])
        {
            edgeVisit[edge->getIndex()] = true;
            // 这里记录自环情况，如果已经发生过一次自环计算了，那么就不能发生第二次了
            int targetWeight = (vertexVisit[target] > 0) ? 0 : scc->getVertexWeight(target);
            sccDistance[start][target] = std::max(sccDistance[start][target], step + 1 + targetWeight);
            dfsSccDistance(scc, start, target, step + 1 + targetWeight, edgeVisit, vertexVisit, sccDistance);
            edgeVisit[edge->getIndex()] = false;
        }
    }
    vertexVisit[cur]--;
}

int WordChain::Loop::dpSolve(char head, char tail, Graph *rawGraph, int sccDistance[30][30],
                             int dp[], Edge *hostPreEdge[], int sccPreVertex[])
{
    // 利用 weight 给 dpSolve 赋初值
    const int *weights = rawGraph->getVertexWeights();
    // 根据 head 来判断从哪里开始，那么只有一个或者全部的节点可以使得 dpSolve 为正数
    // 对于 head 没有限制
    if (head == 0)
    {
        for (int i = 0; i < MAX_VERTEX; i++)
        {
            dp[i] = weights[i];
        }
    }
    else
    {
        int x = head - 'a';
        dp[x] = weights[x];
    }

    // 对于 hostGraph 进行 topoSort 排序
    auto hostGraph = rawGraph->getHostGraph();
    int sccsNum = rawGraph->getSccsNum();
    int topo[MAX_VERTEX];
    hostGraph->topoSort(sccsNum, topo);

    // 应该是按照 topo 序遍历节点
    for (int i = 0; i < sccsNum; i++)
    {
        int scc = topo[i];

        // 首先处理一个 scc 内部的节点，利用之前得到的 sccDistance 将 gp 更新
        // gpVertex[v] = u 表示路径 u -> v
        int gp[MAX_VERTEX], gpVertex[MAX_VERTEX];
        memset(gp, 255, sizeof(int) * MAX_VERTEX);
        memset(gpVertex, 255, sizeof(int) * MAX_VERTEX);
        // 底下这个循环会枚举在 scc 这个强连通分量中的`可能起点`为起点的所有边，然后用 gp[k] 记录以 k 为终点的最短路径
        for (int u = 0; u < ALPHA_SIZE; u++)
        {
            if (rawGraph->getSccIndex()[u] == scc)
            {
                // 只有可以被作为起点的被考虑
                if (dp[u] >= 0)
                {
                    for (int v = 0; v < ALPHA_SIZE; v++)
                    {
                        if (rawGraph->getSccIndex()[v] == scc)
                        {
                            if (dp[u] + sccDistance[u][v] > gp[v])
                            {
                                gp[v] = dp[u] + sccDistance[u][v];
                                gpVertex[v] = u;
                            }
                        }
                    }
                }
            }
        }

        // 利用 gp 更新 dpSolve
        for (int j = 0; j < ALPHA_SIZE; j++)
        {
            if (dp[j] < gp[j])
            {
                dp[j] = gp[j];
                sccPreVertex[j] = gpVertex[j];
            }
        }

        // outside graph
        // 使用 dpSolve 的思想去更新结果，这里利用的形式是 topo 序靠前的 scc 中的每一个节点都会先于 topo 序靠后的节点
        auto hostEdges = rawGraph->getHostGraph()->getVertexEdges()[scc];
        for (auto &edge : hostEdges)
        {
            // 因为这是 host 的边，所以需要用 word 确定单词
            auto word = edge->getWord();
            int source = word[0] - 'a';
            int target = word[strlen(word) - 1] - 'a';
            if (dp[source] >= 0)
            {
                int targetWeight = rawGraph->getVertexWeight(target);
                if (dp[target] < dp[source] + 1 + targetWeight)
                {
                    dp[target] = dp[source] + 1 + targetWeight;
                    hostPreEdge[target] = edge;
                }
            }
        }
    }

    // 找出目标路径的终点
    int ans = 0;
    // 说明不限制终点
    if (tail == 0)
    {
        for (int i = 0; i < ALPHA_SIZE; i++)
        {
            if (dp[i] > dp[ans])
            {
                ans = i;
            }
        }
    }
    else
    {
        ans = tail - 'a';
    }
    // 打印 dp 结果
#ifdef DEBUG
    cout << "dp" << endl;
    for (int o = 0; o < ALPHA_SIZE; o++)
        printf("%4c", o + 'a');
    cout << endl;
    for (int o = 0; o < ALPHA_SIZE; o++)
        printf("%4d", dp[o]);
    cout << endl;
    cout << "destination point: " << (char)(ans + 'a') << endl;
#endif
    if (dp[ans] <= 1)
    {
        throw MyException(WORD_NOT_AVAILABLE);
    }

    return ans;
}

int WordChain::Loop::restoreChain(int cur, Edge *hostPreEdge[], const int sccPreVertex[], Graph *rawGraph,
                                  int sccDistance[][MAX_VERTEX], char *result[])
{
    list<char *> chain;
    // 指示是否在强连通分量中
    bool inSCC = false;
    // `hostPreEdge[cur] >= 0` 表示 cur 是沟通两个 scc 的点（这是因为这个程序的边编号是从 1 开始的）
    // `sccPreVertex[cur] >= 0` 表示 cur 前与 scc 中的有联系
    while (hostPreEdge[cur] != nullptr || !inSCC && sccPreVertex[cur] >= 0)
    {
        // 在强连通分量内部还原路径
        if (!inSCC && sccPreVertex[cur] >= 0)
        {
            inSCC = true;
            int pre = sccPreVertex[cur];
            int sccIndex = rawGraph->getSccIndex()[pre];
            // 子图链
            auto subChain = list<char *>();
            wordListLen = 0;
            bool *edgeVisit = new bool[MAX_EDGE];
            memset(edgeVisit, 0, sizeof(bool) * MAX_EDGE);
            int vertexVisit[MAX_VERTEX] = {0};
            // 根绝 sccDistance 绘制从 pre 到 cur 的子图链
            bool r = restoreSubChain(rawGraph->getSccs()[sccIndex], pre, cur, sccDistance[pre][cur],
                                     edgeVisit, vertexVisit, subChain);
            // 如果没有合适的子图链，那么就说明错了
            if (!r)
            {
                throw MyException(WORD_NOT_AVAILABLE);
            }

            // 将子链加入主链，需要先将 subChain 倒序
            subChain.reverse();
            chain.splice(chain.end(), subChain);
            cur = pre;
        }
        else
        {
            // 首先将当前节点的自环加入
            inSCC = false;
            int curWeight = rawGraph->getVertexWeight(cur);
            if (curWeight > 0)
            {
                for (auto &loop : rawGraph->getVertexSelfLoop()[cur])
                {
                    chain.push_back(loop->getWord());
                }
            }

            // 将坍缩图的边加入
            Edge *bridge = hostPreEdge[cur];
            chain.push_back(bridge->getWord());
            // 将 cur 设置成沟通桥的起点
            cur = bridge->getWord()[0] - 'a';
        }
    }

    // 这里还有一次加入过程，头节点的自环
    if (rawGraph->getVertexWeight(cur) > 0)
    {
        for (auto &loop : rawGraph->getVertexSelfLoop()[cur])
        {
            chain.push_back(loop->getWord());
        }
    }

    // 最后遍历 chain，写入 result
    int i = 0;
    for (auto iter = chain.rbegin(); iter != chain.rend(); iter++)
    {
        result[i++] = *iter;
    }

    return (int)chain.size();
}

bool WordChain::Loop::restoreSubChain(Graph *scc, int cur, int dest, int remain,
                                      bool edgeVisit[], int vertexVisit[], list<char *> &subChain)
{
    // 基准情况，说明长度是符合的
    if (remain == 0)
    {
        // 如果和节点相同，那么就将 wordList 的答案写入 subChain 中
        if (cur == dest)
        {
            // 在 end 处插入整个数组
            subChain.insert(subChain.end(), wordList, wordList + wordListLen);
            // 成功了，返回 true
            return true;
        }
        // 失败了返回 false
        else
        {
            return false;
        }
    }

    vertexVisit[cur]++;
    // 遍历所有的边
    for (auto &edge : scc->getVertexEdges()[cur])
    {
        if (!edgeVisit[edge->getIndex()])
        {
            edgeVisit[edge->getIndex()] = true;
            int target = edge->getTarget();
            // lastLen 是为了恢复 wordListLen 使用的
            int lastLen = wordListLen;
            wordList[wordListLen++] = edge->getWord();
            // 和 dfs 处的同理，自环只能计算一次
            int targetWeight = 0;
            if (vertexVisit[target] == 0)
            {
                targetWeight = scc->getVertexWeight(target);
                for (auto &loop : scc->getVertexSelfLoop()[target])
                {
                    wordList[wordListLen++] = loop->getWord();
                }
            }

            // 进行递归，如果发现了与 target 相同的了，那么就也返回 return true
            bool find = restoreSubChain(scc, target, dest, remain - 1 - targetWeight,
                                        edgeVisit, vertexVisit, subChain);
            if (find)
            {
                return true;
            }

            // 恢复 wordCnt 和 visit
            wordListLen = lastLen;
            edgeVisit[edge->getIndex()] = false;
        }
    }
    vertexVisit[cur]--;
    return false;
}

/* ================================ */

int WordChain::NoLoop::mainSolution(char *words[], int wordsLen, char *result[],
                                    char head, char tail, char ban)
{
    auto rawGraph = new Graph(words, wordsLen);
    // 删除 ban 的单词
    rawGraph->delBannedEdges(ban);

    // dp
    int dp[MAX_VERTEX];
    memset(dp, 255, sizeof(dp));
    Edge* preEdge[MAX_VERTEX] = {nullptr};
    int dest = dpSolve(head, tail, rawGraph, dp, preEdge);

    // 复原链路
    int resultLen = restoreChain(dest, preEdge, rawGraph, result);
    delete rawGraph;
    if (resultLen <= 1)
    {
        throw MyException(WORD_NOT_AVAILABLE);
    }
    return resultLen;
}

int WordChain::NoLoop::dpSolve(char head, char tail, Graph *rawGraph, int dp[], Edge* preEdge[])
{
    // 进行拓扑排序，有两个作用，一个是检测是否有环，另一个是确定 dp 顺序
    int topo[MAX_VERTEX];
    rawGraph->topoSort(ALPHA_SIZE, topo);

    // 利用 head 和 ban 确定 dp 起始点，利用 weight 给 dpSolve 赋初值
    const int *weights = rawGraph->getVertexWeights();
    if (head == 0)
    {
        for (int i = 0; i < ALPHA_SIZE; i++)
        {
            dp[i] = weights[i];
        }
    }
    else
    {
        int x = head - 'a';
        dp[x] = weights[x];
    }

    // 按照 topo 序 dp
    for (int i = 0; i < ALPHA_SIZE; i++)
    {
        int v = topo[i];
        if (dp[v] >= 0)
        {
            for (auto& edge : rawGraph->getVertexEdges()[v])
            {
                int target = edge->getTarget();
                // targetWeight 是需要考虑的，因为自环并不算是，但是两个自环需要排除
                int targetWeight = rawGraph->getVertexWeight(v);
                if (dp[v] + targetWeight + 1 > dp[target])
                {
                    dp[target] = dp[v] + targetWeight + 1;
                    preEdge[target] = edge;
                }
            }
        }
    }

    int dest = 0;
    if (tail == 0)
    {
        for (int i = 1; i < ALPHA_SIZE; i++)
        {
            if (dp[i] > dp[dest])
            {
                dest = i;
            }
        }
    }
    else
    {
        dest = tail - 'a';
    }

    return dest;
}

int WordChain::NoLoop::restoreChain(int cur, Edge* preEdge[], Graph *rawGraph, char *result[])
{
    vector<char *> chain;
    // 普通的 dp 回溯
    while (preEdge[cur] != nullptr)
    {
        int curWeight = rawGraph->getVertexWeight(cur);
        if (curWeight > 0)
        {
            for (auto *loop : rawGraph->getVertexSelfLoop()[cur])
            {
                chain.push_back(loop->getWord());
            }
        }

        auto edge = preEdge[cur];
        chain.push_back(edge->getWord());
        cur = edge->getWord()[0] - 'a';
    }

    if (rawGraph->getVertexWeight(cur) > 0)
    {
        for (auto *loop : rawGraph->getVertexSelfLoop()[cur])
        {
            chain.push_back(loop->getWord());
        }
    }

    int i = 0;
    for (auto iter = chain.rbegin(); iter != chain.rend(); iter++)
    {
        result[i++] = *iter;
    }

    return (int) chain.size();
}

/* ================================ */

int CharChain::Loop::mainSolution(char *words[], int wordsLen, char *result[],
                                  char head, char tail, char ban)
{
    auto rawGraph = new Graph(words, wordsLen);
    // 删除 ban 的单词
    rawGraph->delBannedEdges(ban);

    // 删除孤立边和自环，避免干扰答案
    rawGraph->delSingleEdges();

    // 进行 scc 压缩
    rawGraph->compressGraph();

    // 利用 dfs 去求解每一个 scc 中的距离
    // sccDistance[i][j] 表示由 i -> j 的最大距离，其中不包括 i 上的自环，如果 i, j 位于同一个 scc，如果不再，则为负数
    int sccDistance[MAX_VERTEX][MAX_VERTEX];
    memset(sccDistance, 255, sizeof(sccDistance));
    getSccDistance(rawGraph, sccDistance);

    // 利用 dpSolve 去求解 hostGraph 中的距离
    // dpSolve[i] 表示以 i 为结尾的单词链的最大长度
    int dp[MAX_VERTEX];
    memset(dp, 255, sizeof(dp));
    // 2 个 pre 数组用于还原路径，至于为啥只有 host 是记录边的，而 scc 内是记录点的，是因为 scc 内用 dfs 没法记录边
    // 在 scc 内还原路径时还需要重新 dfs
    int sccPreVertex[MAX_VERTEX];
    memset(sccPreVertex, 255, sizeof(int) * MAX_VERTEX);
    Edge *hostPreEdge[MAX_VERTEX] = {nullptr};
    // dest 是最终路径终点的编号
    int dest = dpSolve(head, tail, rawGraph, sccDistance, dp, hostPreEdge, sccPreVertex);

    // 根据 dest 和两个 pre 数组还原路径
    int resultLen = restoreChain(dest, hostPreEdge, sccPreVertex, rawGraph, sccDistance, result);
    delete rawGraph;
    return resultLen;
}

void CharChain::Loop::getSccDistance(Graph *rawGraph, int sccDistance[][MAX_VERTEX])
{
    // 显然每个点到自己的距离都是 0，因为本身的自环是不计算的
    for (int i = 0; i < ALPHA_SIZE; i++)
    {
        sccDistance[i][i] = 0;
    }

    int sccsNum = rawGraph->getSccsNum();
    // 遍历每一种颜色
    for (int i = 0; i < sccsNum; i++)
    {
        // 遍历每一个节点
        for (int j = 0; j < ALPHA_SIZE; j++)
        {
            // 如果该节点属于某个颜色
            if (rawGraph->getSccIndex()[j] == i)
            {
                bool *edgeVisit = new bool[MAX_EDGE];
                memset(edgeVisit, 0, sizeof(bool) * MAX_EDGE);
                int vertexVisit[MAX_VERTEX] = {0};
                // 进行 dfs
                dfsSccDistance(rawGraph->getSccs()[i], j, j, 0, edgeVisit, vertexVisit, sccDistance);
                delete[] edgeVisit;
            }
        }
    }

    // 打印 sccDistance 表格
#ifdef DEBUG
    printf("    ");
    for (int i = 0; i < ALPHA_SIZE; i++)
    {
        printf("%4c", i + 'a');
    }
    cout << endl;
    for (int i = 0; i < ALPHA_SIZE; i++)
    {
        printf("%4c", i + 'a');
        for (int j = 0; j < ALPHA_SIZE; j++)
        {
            printf("%4d", sccDistance[i][j]);
        }
        cout << endl;
    }
#endif
}

void CharChain::Loop::dfsSccDistance(Graph *scc, int start, int cur, int step,
                                     bool edgeVisit[], int vertexVisit[], int sccDistance[][MAX_VERTEX])
{
    vertexVisit[cur]++;
    // 遍历当前节点的每一条非自环边
    for (auto &edge : scc->getVertexEdges()[cur])
    {
        int target = edge->getTarget();
        if (!edgeVisit[edge->getIndex()])
        {
            edgeVisit[edge->getIndex()] = true;
            // 这里记录自环情况，如果已经发生过一次自环计算了，那么就不能发生第二次了
            int targetWeight = (vertexVisit[target] > 0) ? 0 : scc->getVertexCharWeight(target);
            int edgeWeight = edge->getWeight();
            sccDistance[start][target] = std::max(sccDistance[start][target], step + edgeWeight + targetWeight);
            dfsSccDistance(scc, start, target, step + edgeWeight + targetWeight,
                           edgeVisit, vertexVisit, sccDistance);
            edgeVisit[edge->getIndex()] = false;
        }
    }
    vertexVisit[cur]--;
}

int CharChain::Loop::dpSolve(char head, char tail, Graph *rawGraph, int sccDistance[30][30],
                             int dp[], Edge *hostPreEdge[], int sccPreVertex[])
{
    // 利用 weight 给 dpSolve 赋初值
    const int *weights = rawGraph->getVertexCharWeights();

    // 对于 head 没有限制
    if (head == 0)
    {
        for (int i = 0; i < MAX_VERTEX; i++)
        {
            dp[i] = weights[i];
        }
    }
    else
    {
        int x = head - 'a';
        dp[x] = weights[x];
    }

    // 对于 hostGraph 进行 topoSort 排序
    auto hostGraph = rawGraph->getHostGraph();
    int sccsNum = rawGraph->getSccsNum();
    int topo[MAX_VERTEX];
    hostGraph->topoSort(sccsNum, topo);

    // 应该是按照 topo 序遍历节点
    for (int i = 0; i < sccsNum; i++)
    {
        int scc = topo[i];

        // 首先处理一个 scc 内部的节点，利用之前得到的 sccDistance 将 gp 更新
        // gpVertex[v] = u 表示路径 u -> v
        int gp[MAX_VERTEX];
        int gpVertex[MAX_VERTEX];
        memset(gp, 255, sizeof(gp));
        memset(gpVertex, 255, sizeof(gpVertex));
        // 底下这个循环会枚举在 scc 这个强连通分量中的`可能起点`为起点的所有边，然后用 gp[k] 记录以 k 为终点的最短路径
        for (int u = 0; u < ALPHA_SIZE; u++)
        {
            if (rawGraph->getSccIndex()[u] == scc)
            {
                // 只有可以被作为起点的被考虑
                if (dp[u] >= 0)
                {
                    for (int v = 0; v < ALPHA_SIZE; v++)
                    {
                        if (rawGraph->getSccIndex()[v] == scc)
                        {
                            if (dp[u] + sccDistance[u][v] > gp[v])
                            {
                                gp[v] = dp[u] + sccDistance[u][v];
                                gpVertex[v] = u;
                            }
                        }
                    }
                }
            }
        }

        // 利用 gp 更新 dpSolve
        for (int j = 0; j < ALPHA_SIZE; j++)
        {
            if (dp[j] < gp[j])
            {
                dp[j] = gp[j];
                sccPreVertex[j] = gpVertex[j];
            }
        }

        // outside graph
        // 使用 dpSolve 的思想去更新结果，这里利用的形式是 topo 序靠前的 scc 中的每一个节点都会先于 topo 序靠后的节点
        auto hostEdges = rawGraph->getHostGraph()->getVertexEdges()[scc];
        for (auto &edge : hostEdges)
        {
            // 因为这是 host 的边，所以需要用 word 确定单词
            auto word = edge->getWord();
            int source = word[0] - 'a';
            int target = word[strlen(word) - 1] - 'a';
            if (dp[source] >= 0)
            {
                int targetWeight = rawGraph->getVertexCharWeight(target);
                int edgeWeight = edge->getWeight();
                if (dp[target] < dp[source] + edgeWeight + targetWeight)
                {
                    dp[target] = dp[source] + edgeWeight + targetWeight;
                    hostPreEdge[target] = edge;
                }
            }
        }
    }

    // 找出目标路径的终点
    int ans = 0;
    // 说明不限制终点
    if (tail == 0)
    {
        for (int i = 0; i < ALPHA_SIZE; i++)
        {
            if (dp[i] > dp[ans])
            {
                ans = i;
            }
        }
    }
    else
    {
        ans = tail - 'a';
    }
    // 打印 dp 结果
#ifdef DEBUG
    cout << "dp" << endl;
    for (int o = 0; o < ALPHA_SIZE; o++)
        printf("%4c", o + 'a');
    cout << endl;
    for (int o = 0; o < ALPHA_SIZE; o++)
        printf("%4d", dp[o]);
    cout << endl;
    cout << "destination point: " << (char)(ans + 'a') << endl;
#endif
    return ans;
}

int CharChain::Loop::restoreChain(int cur, Edge *hostPreEdge[], const int sccPreVertex[], Graph *rawGraph,
                                  int sccDistance[][MAX_VERTEX], char *result[])
{
    list<char *> chain;
    // 指示是否在强连通分量中
    bool inSCC = false;
    // `hostPreEdge[cur] >= 0` 表示 cur 是沟通两个 scc 的点（这是因为这个程序的边编号是从 1 开始的）
    // `sccPreVertex[cur] >= 0` 表示 cur 前与 scc 中的有联系
    while (hostPreEdge[cur] != nullptr || !inSCC && sccPreVertex[cur] >= 0)
    {
        // 在强连通分量内部还原路径
        if (!inSCC && sccPreVertex[cur] >= 0)
        {
            inSCC = true;
            int pre = sccPreVertex[cur];
            int sccIndex = rawGraph->getSccIndex()[pre];
            // 子图链
            auto subChain = list<char *>();
            wordListLen = 0;
            bool *edgeVisit = new bool[MAX_EDGE];
            memset(edgeVisit, 0, sizeof(bool) * MAX_EDGE);
            int vertexVisit[MAX_VERTEX] = {0};
            // 根据 sccDistance 绘制从 pre 到 cur 的子图链
            bool r = restoreSubChain(rawGraph->getSccs()[sccIndex], pre, cur, sccDistance[pre][cur],
                                     edgeVisit, vertexVisit, subChain);
            // 如果没有合适的子图链，那么就说明错了
            if (!r)
            {
                throw MyException(WORD_NOT_AVAILABLE);
            }

            // 将子链加入主链，需要先将 subChain 倒序
            subChain.reverse();
            chain.splice(chain.end(), subChain);
            cur = pre;
        }
        else
        {
            // 首先将当前节点的自环加入
            inSCC = false;
            int curWeight = rawGraph->getVertexCharWeight(cur);
            if (curWeight > 0)
            {
                for (auto &loop : rawGraph->getVertexSelfLoop()[cur])
                {
                    chain.push_back(loop->getWord());
                }
            }

            // 将坍缩图的边加入
            Edge *bridge = hostPreEdge[cur];
            chain.push_back(bridge->getWord());
            // 将 cur 设置成沟通桥的起点
            cur = bridge->getWord()[0] - 'a';
        }
    }

    // 这里还有一次加入过程，头节点的自环
    if (rawGraph->getVertexWeight(cur) > 0)
    {
        for (auto &loop : rawGraph->getVertexSelfLoop()[cur])
        {
            chain.push_back(loop->getWord());
        }
    }

    // 最后遍历 chain，写入 result
    int i = 0;
    for (auto iter = chain.rbegin(); iter != chain.rend(); iter++)
    {
        result[i++] = *iter;
    }

    return (int)chain.size();
}

bool CharChain::Loop::restoreSubChain(Graph *scc, int cur, int dest, int remain,
                                      bool edgeVisit[], int vertexVisit[], list<char *> &subChain)
{
    // 基准情况，说明长度是符合的
    if (remain == 0)
    {
        // 如果和节点相同，那么就将 wordList 的答案写入 subChain 中
        if (cur == dest)
        {
            // 在 end 处插入整个数组
            subChain.insert(subChain.end(), wordList, wordList + wordListLen);
            // 成功了，返回 true
            return true;
        }
        // 失败了返回 false
        else
        {
            return false;
        }
    }

    vertexVisit[cur]++;
    // 遍历所有的边
    for (auto &edge : scc->getVertexEdges()[cur])
    {
        if (!edgeVisit[edge->getIndex()])
        {
            edgeVisit[edge->getIndex()] = true;
            int target = edge->getTarget();
            // lastLen 是为了恢复 wordListLen 使用的
            int lastLen = wordListLen;
            wordList[wordListLen++] = edge->getWord();
            // 和 dfs 处的同理，自环只能计算一次
            int targetWeight = 0;
            if (vertexVisit[target] == 0)
            {
                targetWeight = scc->getVertexCharWeight(target);
                for (auto &loop : scc->getVertexSelfLoop()[target])
                {
                    wordList[wordListLen++] = loop->getWord();
                }
            }
            int edgeWeight = edge->getWeight();
            // 进行递归，如果发现了与 target 相同的了，那么就也返回 return true
            bool find = restoreSubChain(scc, target, dest, remain - edgeWeight - targetWeight,
                                        edgeVisit, vertexVisit, subChain);
            if (find)
            {
                return true;
            }

            // 恢复 wordLen 和 visit
            wordListLen = lastLen;
            edgeVisit[edge->getIndex()] = false;
        }
    }
    vertexVisit[cur]--;
    return false;
}

/* ================================ */

int CharChain::NoLoop::mainSolution(char *words[], int wordsLen, char *result[],
                                    char head, char tail, char ban)
{
    auto rawGraph = new Graph(words, wordsLen);
    // 删除 ban 的单词
    rawGraph->delBannedEdges(ban);

    // 同样需要删去容易造成干扰的边
    rawGraph->delSingleEdges();

    // dp
    int dp[MAX_VERTEX];
    memset(dp, 255, sizeof(dp));
    Edge* preEdge[MAX_VERTEX] = {nullptr};
    int dest = dpSolve(head, tail, rawGraph, dp, preEdge);

    // 复原链路
    int resultLen = restoreChain(dest, preEdge, rawGraph, result);
    delete rawGraph;
    if (resultLen <= 1)
    {
        throw MyException(WORD_NOT_AVAILABLE);
    }
    return resultLen;
}

int CharChain::NoLoop::dpSolve(char head, char tail, Graph *rawGraph, int dp[], Edge* preEdge[])
{
    // 进行拓扑排序，有两个作用，一个是检测是否有环，另一个是确定 dp 顺序
    int topo[MAX_VERTEX];
    rawGraph->topoSort(ALPHA_SIZE, topo);

    // 利用 head 和 ban 确定 dp 起始点，利用 weight 给 dpSolve 赋初值
    const int *weights = rawGraph->getVertexCharWeights();
    if (head == 0)
    {
        for (int i = 0; i < ALPHA_SIZE; i++)
        {
            dp[i] = weights[i];
        }
    }
    else
    {
        int x = head - 'a';
        dp[x] = weights[x];
    }

    // 按照 topo 序 dp
    for (int i = 0; i < ALPHA_SIZE; i++)
    {
        int v = topo[i];
        if (dp[v] >= 0)
        {
            for (auto& edge : rawGraph->getVertexEdges()[v])
            {
                int target = edge->getTarget();
                // targetWeight 是需要考虑的，因为自环并不算是，但是两个自环需要排除
                // TODO 问问助教
                int targetWeight = rawGraph->getVertexCharWeight(v);
                int edgeWeight = edge->getWeight();
                if (dp[v] + targetWeight + edgeWeight > dp[target])
                {
                    dp[target] = dp[v] + targetWeight + edgeWeight;
                    preEdge[target] = edge;
                }
            }
        }
    }

    int dest = 0;
    if (tail == 0)
    {
        for (int i = 1; i < ALPHA_SIZE; i++)
        {
            if (dp[i] > dp[dest])
            {
                dest = i;
            }
        }
    }
    else
    {
        dest = tail - 'a';
    }

    return dest;
}

int CharChain::NoLoop::restoreChain(int cur, Edge* preEdge[], Graph *rawGraph, char *result[])
{
    vector<char *> chain;
    // 普通的 dp 回溯
    while (preEdge[cur] != nullptr)
    {
        int curWeight = rawGraph->getVertexCharWeight(cur);
        if (curWeight > 0)
        {
            for (auto *loop : rawGraph->getVertexSelfLoop()[cur])
            {
                chain.push_back(loop->getWord());
            }
        }

        auto edge = preEdge[cur];
        chain.push_back(edge->getWord());
        cur = edge->getWord()[0] - 'a';
    }

    if (rawGraph->getVertexCharWeight(cur) > 0)
    {
        for (auto *loop : rawGraph->getVertexSelfLoop()[cur])
        {
            chain.push_back(loop->getWord());
        }
    }

    int i = 0;
    for (auto iter = chain.rbegin(); iter != chain.rend(); iter++)
    {
        result[i++] = *iter;
    }

    return (int) chain.size();
}
