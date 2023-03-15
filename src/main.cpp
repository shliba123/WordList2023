//
// Created by shliba on 2023/3/7.
//
#include "./control/control.h"
#include "gtest/gtest.h"
#include <gperftools/profiler.h>

int main(int argc, char *argv[])
{
    ProfilerStart("test.prof");
    for (int i = 0; i < 1; i++)
    {
        control(argc, argv);
    }
    ProfilerStop();
    return 0;
}
