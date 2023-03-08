//
// Created by shliba on 2023/3/7.
//
#include "iostream"
#include "./control/parse.h"
#include "./control/util.h"

using namespace std;

int main(int argc, char *argv[])
{
    try
    {
        auto ans = parseParam(argc, argv);
        cout << get<0>(ans) << " " << get<1>(ans) << " " << get<2>(ans) << " " << get<3>(ans) << " " << get<4>(ans) << " " << get<4>(ans) << " " << endl;
    }
    catch (MyException &e)
    {
        e.what();
    }

    return 0;
}
