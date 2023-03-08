//
// Created by thysrael on 23-3-8.
//

#include "parse.h"
#include "util.h"

std::tuple<int, int, int, int, bool, char *> parseParam(int argc, char *argv[])
{
    char *path = nullptr;
    int problemType = WRONG_PROBLEM;
    int head = 0;
    int tail = 0;
    int ban = 0;
    bool allowLoop = false;

    for (int i = 0; i < argc; i++)
    {
        if (isFilePath(argv[i]))
        {
            if (path != nullptr)
            {
                throw MyException(MULTI_FILE_PATH);
            }
            else
            {
                path = argv[i];
            }
        }
        // 解析参数
        else if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 'n':
                if (problemType != WRONG_PROBLEM)
                {
                    throw MyException(MULTI_FILE_PATH);
                }
                problemType = COUNT_CHAINS;
                break ;

            case 'w':
                if (problemType != WRONG_PROBLEM)
                {
                    throw MyException(MULTI_FILE_PATH);
                }
                problemType = MAX_CHAIN_WORD;
                break;

            case 'c':
                if (problemType != WRONG_PROBLEM)
                {
                    throw MyException(MULTI_FILE_PATH);
                }
                problemType = MAX_CHAIN_CHAR;
                break;

            case 'h':
                if (i == argc - 1)
                {
                    throw MyException(NO_CHAR_ERROR);
                }
                if (isParamAlpha(argv[++i]))
                {
                    head = (int) (unsigned char) argv[i][0];
                }
                else
                {
                    throw MyException(NO_CHAR_ERROR);
                }
                break;

            case 't':
                if (i == argc - 1)
                {
                    throw MyException(NO_CHAR_ERROR);
                }
                if (isParamAlpha(argv[++i]))
                {
                    tail = (int) (unsigned char) argv[i][0];
                }
                else
                {
                    throw MyException(NO_CHAR_ERROR);
                }
                break;

            case 'j':
                if (i == argc - 1)
                {
                    throw MyException(NO_CHAR_ERROR);
                }
                if (isParamAlpha(argv[++i]))
                {
                    ban = (int) (unsigned char) argv[i][0];
                }
                else
                {
                    throw MyException(NO_CHAR_ERROR);
                }
                break;

            case 'r':
                if (allowLoop)
                {
                    throw MyException(ENABLE_LOOP_DUPLICATE);
                }
                allowLoop = true;
                break;

            default:
                throw MyException(PARAMETER_NOT_EXISTS);
            }
        }
    }

    if (path == nullptr)
    {
        throw MyException(NO_FILE_PATH);
    }
    if ((head > 0 || tail > 0 || ban > 0 || allowLoop) && (problemType == COUNT_CHAINS))
    {
        throw MyException(N_WORK_WITH_OTHER_PARAMETER);
    }
    if (problemType == WRONG_PROBLEM)
    {
        throw MyException(NO_WORK_ERROR);
    }
    if (ban == head)
    {
        throw MyException(WORD_NOT_AVAILABLE);
    }

    return {problemType, head, tail, ban, allowLoop, path};
}

static bool inline isFilePath(char *str)
{
    size_t len = strlen(str);
    // 判断后缀是否是 .txt
    if (len > 4) {
        return str[len - 1] == 't' && str[len - 2] == 'x' && str[len - 3] == 't' && str[len - 4] == '.';
    } else {
        return false;
    }
}

static bool inline isParamAlpha(char *str)
{
    return strlen(str) == 1 && isalpha(str[0]);
}