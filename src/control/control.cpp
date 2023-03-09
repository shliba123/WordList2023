//
// Created by thysrael on 23-3-8.
//
#include "iostream"
#include "control.h"
#include "io.h"
#include "../core/core.h"

using std::cout;
using  std::get;
using std::endl;
using std::cerr;

const int MAX_WORD = 20100;
char* words[MAX_WORD];
void control(int argc, char *argv[])
{
    try
    {
        auto ans = parseParam(argc, argv);
        cout << get<0>(ans) << " " << get<1>(ans) << " " << get<2>(ans) << " " << get<3>(ans) << " " << get<4>(ans) << " " << get<4>(ans) << " " << endl;
        char *path = get<5>(ans);
        int problemType = get<0>(ans);
        int head = get<1>(ans);
        int tail = get<2>(ans);
        int ban = get<3>(ans);
        bool allowLoop = get<4>(ans);
        auto wordCount = readWords(path, words);
        for (int i = 0; i < wordCount; ++i) {
            cout << words[i] << endl;
        }
        cout << wordCount << endl;
        char* result[10005];
        auto resultLegth = countChains(words, wordCount, result);
        for (int i = 0; i < resultLegth; ++i) {
            cout << result[i] << std::endl;
        }
    }
    catch (MyException &e)
    {
        cerr << e.what();
    }
}