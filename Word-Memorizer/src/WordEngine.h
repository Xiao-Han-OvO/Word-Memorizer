#pragma once

#include <QSet>
#include <QString>
#include <QUrl>
#include <QVector>
#include <random>
#include <algorithm>

struct WordEntry {
    QString word;
    QString pos;
    QString meaning;
    QString example;

    bool isValid() const { return !word.trimmed().isEmpty(); }
};

class WordEngine {
public:
    WordEngine();

    bool loadFromFile(const QString &fileName, QString *errorMessage = nullptr);
    bool saveToFile(const QString &fileName, const QVector<WordEntry> &entries, QString *errorMessage = nullptr) const;
    bool exportWrongWords(const QString &fileName, QString *errorMessage = nullptr) const;

    static QVector<WordEntry> readWordFile(const QString &fileName, QString *errorMessage = nullptr);
    static QString normalizeFileUrl(const QUrl &url);

    WordEntry nextWord();
    void markWrong(const WordEntry &entry);
    void markFailed(const QString &word);
    void markMastered(const QString &word);
    void clearWrongWords();
    void clearProgress();

    const QVector<WordEntry>& allWords() const { return m_words; }
    QVector<WordEntry> wrongWords() const;

    int totalCount() const { return m_words.size(); }
    int masteredCount() const { return m_masteredWords.size(); }
    int wrongCount() const { return m_wrongKeys.size(); }
    int remainingCount() const { return std::max(0, totalCount() - masteredCount()); }
    bool hasWords() const { return !m_words.isEmpty(); }
    bool allMastered() const { return hasWords() && masteredCount() >= totalCount(); }
    double progress() const { return totalCount() == 0 ? 0.0 : double(masteredCount()) / double(totalCount()); }
    bool hasFailed(const QString &word) const { return m_failedWords.contains(word.toLower()); }

private:
    static QString keyFor(const WordEntry &entry);
    static QString serialize(const WordEntry &entry);

    QVector<WordEntry> m_words;
    QSet<QString> m_wrongKeys;
    QSet<QString> m_masteredWords;
    QSet<QString> m_failedWords;
    std::mt19937 m_rng;
};
