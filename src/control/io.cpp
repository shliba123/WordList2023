//
// Created by thysrael on 23-3-8.
//

#include "io.h"

int readWords(char *inputFilePath, char *words[]) {
    FILE *file;
    file = fopen(inputFilePath, "rb");
    if (file == nullptr) {
        throw MyException(FILE_NOT_FIND);
    }
    fseek(file, 0, SEEK_END);
    int len = (int) ftell(file);
    auto context = (char *) malloc(len + 1);
    fseek(file, 0, SEEK_SET);
    auto readLen = fread(context, 1, (size_t) len + 1, file);
    context[readLen] = '\0';
    fclose(file);
    std::unordered_set<std::string> wordSet;
    int cnt = 0;
    bool flag = false;
    int prev = 0;
    for (int i = 0; i < readLen; ++i) {
        if (isalpha(context[i])) {
            if (!flag) {
                flag = true;
                prev = i;
            }
            context[i] = (char) tolower(context[i]);
        } else {
            context[i] = '\0';
            auto currentWord = std::string(&context[prev]);
            if (flag && !wordSet.count(currentWord)) {
                wordSet.insert(currentWord);
                words[cnt++] = &context[prev];
            }
            flag = false;
        }

    }
    if (flag) {
        context[readLen] = '\0';
        auto currentWord = std::string(&context[prev]);
        if (flag && !wordSet.count(currentWord)) {
            wordSet.insert(currentWord);
            words[cnt++] = &context[prev];
        }
    }
    return cnt;
}


