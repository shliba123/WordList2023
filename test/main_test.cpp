//
// Created by shliba on 2023/3/10.
//
#include "gtest/gtest.h"
#include "../src/core/core.h"

void testCountChains(char *words[], int len, char *ans[], int ans_len) {
    char **result = (char **) malloc(10000);
    int out_len = countChains(words, ans_len, result);
    ASSERT_TRUE(ans_len == out_len);

    std::sort(result, result + out_len, [](char *
    p, char *q) { return strcmp(p, q) < 0; });

    std::sort(ans, ans + ans_len, [](char *
    p, char *q) { return strcmp(p, q) < 0; });


    for (int i = 0; i < ans_len; i++) {
        if (result != nullptr) ASSERT_TRUE(strcmp(ans[i], result[i]) == 0);
    }
}

void testgetLongestWordChain(char *words[], int len, char *ans[], int ans_len, char head, char tail, char ban,
                             bool allowLoop) {
    char **result = (char **) malloc(10000);
    int out_len = 0;
    try {
        out_len = getLongestWordChain(words, len, result, head, tail, ban, allowLoop);
    } catch (MyException &e) {
        std::cerr << e.what() << endl;
    }
//    for (int i = 0; i < out_len; ++i) {
//        std::cout << result[i] << std::endl;
//    }
//    std::cout << endl;
//    for (int i = 0; i < ans_len; ++i) {
//        std::cout << ans[i] << std::endl;
//    }
    ASSERT_TRUE(ans_len == out_len);

    std::sort(result, result + out_len, [](char *
    p, char *q) { return strcmp(p, q) < 0; });

    std::sort(ans, ans + ans_len, [](char *
    p, char *q) { return strcmp(p, q) < 0; });
//
//    for (int i = 0; i < ans_len; i++) {
//        if (result != nullptr) ASSERT_TRUE(strcmp(ans[i], result[i]) == 0);
//    }
}

void testgetLongestCharChain(char *words[], int len, char *ans[], int ans_len, char head, char tail,
                             char ban, bool allowLoop) {
    char **result = (char **) malloc(10000);
    int out_len = 0;
    try {
        out_len = getLongestCharChain(words, len, result, head, tail, ban, allowLoop);
    } catch (MyException &e) {
        std::cerr << e.what();
    }

    ASSERT_TRUE(ans_len == out_len);

    std::sort(result, result + out_len, [](char *
    p, char *q) { return strcmp(p, q) < 0; });

    std::sort(ans, ans + ans_len, [](char *
    p, char *q) { return strcmp(p, q) < 0; });
//
    for (int i = 0; i < ans_len; i++) {
        if (result != nullptr) ASSERT_TRUE(strcmp(ans[i], result[i]) == 0);
    }
}


int add(int a, int b) {
    return a + b;
}

TEST(countChains, zero) {
    char *words[] = {"all", "llb", "be"};
    char *ans[] = {"all llb", "llb be", "all llb be"};
    testCountChains(words, 3, ans, 3);
}

TEST(getLongestWordChain, w) {
    char *words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick",
                     "pseudopseudohypoparathyroidism"};
    char *ans[] = {"algebra", "apple", "elephant", "trick"};
    testgetLongestWordChain(words, 11, ans, 4, 0, 0, 0, false);
}

TEST(getLongestWordChain, h) {
    char *words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick",
                     "pseudopseudohypoparathyroidism"};
    char *ans[] = {"elephant", "trick"};
    testgetLongestWordChain(words, 11, ans, 2, 'e', 0, 0, false);
}

TEST(getLongestWordChain, t) {
    char *words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick",
                     "pseudopseudohypoparathyroidism"};
    char *ans[] = {"algebra", "apple", "elephant"};
    testgetLongestWordChain(words, 11, ans, 3, 0, 't', 0, false);
}


TEST(getLongestWordChain, t_h) {
    char *words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick",
                     "pseudopseudohypoparathyroidism"};
    char *ans[] = {"algebra", "apple", "elephant"};
    testgetLongestWordChain(words, 11, ans, 3, 'a', 't', 0, false);
}

TEST(getLongestWordChain, scc) {
    char *words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick",
                     "pseudopseudohypoparathyroidism", "moon", "noom", "nooom", "moooom", "nooooon"};
    char *ans[] = {"nooooon", "noom", "moooom", "moon", "nooom", "mop", "pseudopseudohypoparathyroidism"};
    testgetLongestWordChain(words, 16, ans, 7, 0, 0, 0, true);
}

TEST(getLongestWordChain, sccBan) {
    char *words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick",
                     "pseudopseudohypoparathyroidism", "moon", "noom", "nooom", "moooom", "nooooon"};
    char *ans[] = {"moooom", "moon", "nooom", "mop", "pseudopseudohypoparathyroidism"};
    testgetLongestWordChain(words, 16, ans, 4, 0, 0, 'n', true);
}

TEST(getLongestWordChain, simpleChainAndSingleCharacter) {
    char *words[] = {"ab", "bc", "cd", "de", "ea", "a", "b", "c", "d", "e"};
    char *ans[] = {"a", "ab", "b", "bc", "c", "cd", "d", "de", "e", "ea"};
    testgetLongestWordChain(words, 10, ans, 10, 0, 0, 0, true);
}

TEST(getLongestWordChain, simpleChainAndSingleCharacterHparamTparam) {
    char *words[] = {"ab", "bc", "cd", "de", "ea", "a", "b", "c", "d", "e"};
    char *ans[] = {"a", "ab", "b", "bc", "c", "cd", "d", "de", "e", "ea"};
    testgetLongestWordChain(words, 10, ans, 10, 'a', 'a', 0, true);
}

TEST(getLongestWordChain, singleWord) {
    char *words[] = {"aaa"};
    char *ans[] = {};
    testgetLongestWordChain(words, 1, ans, 0, 'a', 'a', 0, true);
}

TEST(getLongestWordChain, singleWordNoLoop) {
    char *words[] = {"aaa"};
    char *ans[] = {};
    testgetLongestWordChain(words, 1, ans, 0, 'a', 'a', 0, false);
}

TEST(getLongestCharChain, singleWordNoLoop) {
    char *words[] = {"aaa"};
    char *ans[] = {};
    testgetLongestCharChain(words, 1, ans, 0, 'a', 'a', 0, false);
}

TEST(getLongestCharChain, singleWord) {
    char *words[] = {"aaa"};
    char *ans[] = {};
    testgetLongestCharChain(words, 1, ans, 0, 'a', 'a', 0, true);
}

TEST(getLongestCharChain, rparam) {
    char *words[] = {"element", "heaven", "table", "teach", "talk"};
    char *ans[] = {"table", "element", "teach", "heaven"};
    testgetLongestCharChain(words, 5, ans, 4, 0, 0, 0, true);
}

TEST(getLongestCharChain, cparam) {
    char *words[] = {"element", "heaven", "teach", "talk"};
    char *ans[] = {"element", "teach", "heaven"};
    testgetLongestCharChain(words, 4, ans, 3, 0, 0, 0, false);
}

TEST(getLongestChariChain, hparam) {
    char *words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick",
                     "pseudopseudohypoparathyroidism"};
    char *ans[] = {"pseudopseudohypoparathyroidism", "moon"};
    testgetLongestCharChain(words, 11, ans, 2, 'p', 'n', 0, false);
}

TEST(getLongestCharChain, hparamtParamLoop) {
    char *words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick",
                     "pseudopseudohypoparathyroidism"};
    char *ans[] = {"pseudopseudohypoparathyroidism", "moon"};
    testgetLongestCharChain(words, 11, ans, 2, 'p', 'n', 0, true);
}

TEST(getLongestCharChain, hpramTparamLoopNew) {
    char *words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick",
                     "pseudopseudohypoparathyroidism"};
    char *ans[] = {"pseudopseudohypoparathyroidism", "moon"};
    testgetLongestCharChain(words, 11, ans, 2, 'p', 'n', 0, false);
}


TEST(add, zero) {
    EXPECT_EQ(0, add(0, 0)) << "yes";
}

TEST(add, positive_number) {
    EXPECT_EQ(3, add(1, 2));
}

TEST(add, negative_number) {
    EXPECT_EQ(-3, add(-1, -2));
}