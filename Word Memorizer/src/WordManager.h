#ifndef WORDMANAGER_H
#define WORDMANAGER_H

#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_set>

struct Word {
    std::string word;        // 英文单词
    std::string pos;         // 词性 (part of speech)
    std::string meaning;     // 中文意思
    std::string example;     // 例句
    
    // 重载==运算符，用于比较两个Word对象
    bool operator==(const Word& other) const {
        return word == other.word && pos == other.pos && meaning == other.meaning;
    }
};

// 为Word结构体提供哈希函数，用于unordered_set
namespace std {
    template<>
    struct hash<Word> {
        size_t operator()(const Word& w) const {
            return hash<string>()(w.word) ^ 
                   hash<string>()(w.pos) ^ 
                   hash<string>()(w.meaning);
        }
    };
}

class WordManager {
private:
    std::vector<Word> words;
    std::unordered_set<Word> wrongWords;  // 使用unordered_set避免重复
    std::unordered_set<std::string> masteredWords;
    std::mt19937 rng;

public:
    WordManager();
    bool loadWordsFromFile(const std::string& filename);
    Word getRandomWord();
    bool addWrongWord(const Word& word);  // 返回是否成功添加（避免重复）
    void addMasteredWord(const std::string& word);
    std::vector<Word> getWrongWords() const;
    void clearWrongWords();
    void clearMasteredWords();
    size_t getTotalWords() const;
    size_t getWrongWordsCount() const;
    size_t getMasteredWordsCount() const;
    size_t getRemainingWordsCount() const;
    bool allWordsMastered() const;
    float getProgress() const;
};

#endif