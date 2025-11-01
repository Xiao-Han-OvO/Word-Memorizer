#ifndef WORDMANAGER_H
#define WORDMANAGER_H

#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

// 单词数据结构
struct Word {
	std::string word;        // 英文单词
	std::string pos;         // 词性
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
			return std::hash<std::string>()(w.word + "|" + w.pos);
		}
	};
}

// 单词管理器类
class WordManager {
private:
	std::vector<Word> words;                           // 所有单词
	std::unordered_set<Word> wrongWords;               // 错词集合
	std::unordered_set<std::string> masteredWords;     // 已掌握单词
	std::unordered_set<std::string> failedWords;       // 曾经拼错过的单词
	std::mt19937 rng;                                  // 随机数生成器

public:
	WordManager();
	bool loadWordsFromFile(const std::string& filename);
	Word getRandomWord();
	bool addWrongWord(const Word& word);
	void addFailedWord(const std::string& word);
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
	bool hasFailed(const std::string& word) const;
};

#endif