//
// Created by shliba on 2023/3/7.
//
#include "./control/control.h"
#include "gtest/gtest.h"
using namespace std;

int main(int argc, char *argv[])
{
    control(argc, argv);
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
