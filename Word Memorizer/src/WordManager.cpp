#include "WordManager.h"
#include <random>

WordManager::WordManager() : rng(std::random_device{}()) {}

bool WordManager::loadWordsFromFile(const std::string& filename) {
    words.clear();
    wrongWords.clear();
    masteredWords.clear();
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string word, pos, meaning, example;
        
        if (std::getline(ss, word, '|') && 
            std::getline(ss, pos, '|') &&
            std::getline(ss, meaning, '|')) {
            
            std::getline(ss, example);
            
            Word w;
            w.word = word;
            w.pos = pos;
            w.meaning = meaning;
            w.example = example;
            words.push_back(w);
        }
    }
    
    file.close();
    return !words.empty();
}

Word WordManager::getRandomWord() {
    if (words.empty() || allWordsMastered()) {
        return Word{"", "", "", ""};
    }
    
    // 只从未掌握的单词中随机选择
    std::vector<Word> remainingWords;
    for (const auto& word : words) {
        if (masteredWords.find(word.word) == masteredWords.end()) {
            remainingWords.push_back(word);
        }
    }
    
    if (remainingWords.empty()) {
        return Word{"", "", "", ""};
    }
    
    std::uniform_int_distribution<size_t> dist(0, remainingWords.size() - 1);
    return remainingWords[dist(rng)];
}

bool WordManager::addWrongWord(const Word& word) {
    // 检查是否已经存在该错词
    if (wrongWords.find(word) != wrongWords.end()) {
        return false; // 已经存在，不重复添加
    }
    
    wrongWords.insert(word);
    return true; // 成功添加
}

void WordManager::addMasteredWord(const std::string& word) {
    masteredWords.insert(word);
}

std::vector<Word> WordManager::getWrongWords() const {
    return std::vector<Word>(wrongWords.begin(), wrongWords.end());
}

void WordManager::clearWrongWords() {
    wrongWords.clear();
}

void WordManager::clearMasteredWords() {
    masteredWords.clear();
}

size_t WordManager::getTotalWords() const {
    return words.size();
}

size_t WordManager::getWrongWordsCount() const {
    return wrongWords.size();
}

size_t WordManager::getMasteredWordsCount() const {
    return masteredWords.size();
}

size_t WordManager::getRemainingWordsCount() const {
    return words.size() - masteredWords.size();
}

bool WordManager::allWordsMastered() const {
    return !words.empty() && masteredWords.size() >= words.size();
}

float WordManager::getProgress() const {
    if (words.empty()) return 0.0f;
    return static_cast<float>(masteredWords.size()) / words.size();
}
