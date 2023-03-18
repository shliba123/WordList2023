//
// Created by shliba on 2023/3/7.
//
#include "./control/control.h"
#include "gtest/gtest.h"

#include "./control//util.h"

int main(int argc, char *argv[]) {
#ifdef PERF
    ProfilerStart("test.prof");
    for (int i = 0; i < 10000; i++)
    {
        control(argc, argv);
    }
    ProfilerStop();
#else

    control(argc, argv);


    ::testing::InitGoogleTest();
#endif
    return RUN_ALL_TESTS();
}
