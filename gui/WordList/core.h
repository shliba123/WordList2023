#ifndef CORE_H
#define CORE_H
/**
 * 统计所有的单词链
 * @param words 单词数组
 * @param wordsLen 单词数组大小
 * @param result 结果数组，每一个元素是一个单词链
 * @return 单词链的个数
 */
int countChains(char *words[], int wordsLen, char *result[]);

/**
 * 以单词形式统计最长的单词链
 * @param words 单词数组
 * @param wordsLen 单词数组大小
 * @param result 结果数组
 * @param head 指定首字母，如果指定，则范围为 'a' ~ 'z'，若没有指定则为 0
 * @param tail 指定尾字母，如果指定，则范围为 'a' ~ 'z'，若没有指定则为 0
 * @param ban 指定禁用的首字母，如果指定，则范围为 'a' ~ 'z'，若没有指定则为 0
 * @param allowLoop 是否允许环
 * @return 最长的单词链的长度
 */
int getLongestWordChain(char *words[], int wordsLen, char *result[], char head, char tail, char ban, bool allowLoop);

/**
 * 以字符形式统计最长的单词链
 * @param words 单词数组
 * @param wordsLen 单词数组大小
 * @param result 结果数组
 * @param head 指定首字母，如果指定，则范围为 'a' ~ 'z'，若没有指定则为 0
 * @param tail 指定尾字母，如果指定，则范围为 'a' ~ 'z'，若没有指定则为 0
 * @param ban 指定禁用的首字母，如果指定，则范围为 'a' ~ 'z'，若没有指定则为 0
 * @param allowLoop 是否允许环
 * @return 最长的单词链的长度
 */
int getLongestCharChain(char *words[], int wordsLen, char *result[], char head, char tail, char ban, bool allowLoop);

#endif // CORE_H
