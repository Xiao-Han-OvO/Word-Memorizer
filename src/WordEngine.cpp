#include "WordEngine.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QRandomGenerator>

WordEngine::WordEngine()
    : m_rng(std::random_device{}())
{
}

QString WordEngine::normalizeFileUrl(const QUrl &url)
{
    if (url.isLocalFile()) {
        return url.toLocalFile();
    }
    return url.toString(QUrl::PreferLocalFile | QUrl::RemovePassword);
}

QVector<WordEntry> WordEngine::readWordFile(const QString &fileName, QString *errorMessage)
{
    QVector<WordEntry> result;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (errorMessage) *errorMessage = QStringLiteral("无法打开文件：%1").arg(file.errorString());
        return result;
    }

    QTextStream in(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    in.setCodec("UTF-8");
#endif
    int lineNumber = 0;
    while (!in.atEnd()) {
        const QString rawLine = in.readLine();
        ++lineNumber;
        const QString line = rawLine.trimmed();
        if (line.isEmpty() || line.startsWith('#')) continue;

        const QStringList parts = rawLine.split('|');
        if (parts.size() < 3) {
            // 保持宽容：跳过坏行，不中断整个词库读取。
            continue;
        }

        WordEntry entry;
        entry.word = parts.value(0).trimmed();
        entry.pos = parts.value(1).trimmed();
        entry.meaning = parts.value(2).trimmed();
        if (parts.size() >= 4) {
            QStringList exampleParts = parts.mid(3);
            entry.example = exampleParts.join('|').trimmed();
        }

        if (entry.isValid()) {
            result.push_back(entry);
        }
    }

    if (result.isEmpty() && errorMessage) {
        *errorMessage = QStringLiteral("文件中没有可用单词。格式应为：单词|词性|释义|例句");
    }
    return result;
}

bool WordEngine::loadFromFile(const QString &fileName, QString *errorMessage)
{
    const QVector<WordEntry> loaded = readWordFile(fileName, errorMessage);
    if (loaded.isEmpty()) return false;

    m_words = loaded;
    clearWrongWords();
    clearProgress();
    return true;
}

bool WordEngine::saveToFile(const QString &fileName, const QVector<WordEntry> &entries, QString *errorMessage) const
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        if (errorMessage) *errorMessage = QStringLiteral("无法保存文件：%1").arg(file.errorString());
        return false;
    }

    QTextStream out(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    out.setCodec("UTF-8");
#endif
    for (const auto &entry : entries) {
        if (!entry.isValid()) continue;
        out << serialize(entry) << '\n';
    }
    return true;
}

bool WordEngine::exportWrongWords(const QString &fileName, QString *errorMessage) const
{
    return saveToFile(fileName, wrongWords(), errorMessage);
}

WordEntry WordEngine::nextWord()
{
    if (m_words.isEmpty() || allMastered()) return {};

    QVector<WordEntry> remaining;
    remaining.reserve(m_words.size());
    for (const auto &entry : m_words) {
        if (!m_masteredWords.contains(entry.word.toLower())) {
            remaining.push_back(entry);
        }
    }
    if (remaining.isEmpty()) return {};

    std::uniform_int_distribution<int> dist(0, remaining.size() - 1);
    return remaining.at(dist(m_rng));
}

void WordEngine::markWrong(const WordEntry &entry)
{
    if (entry.isValid()) {
        m_wrongKeys.insert(keyFor(entry));
    }
}

void WordEngine::markFailed(const QString &word)
{
    if (!word.trimmed().isEmpty()) {
        m_failedWords.insert(word.toLower());
    }
}

void WordEngine::markMastered(const QString &word)
{
    if (!word.trimmed().isEmpty()) {
        m_masteredWords.insert(word.toLower());
        m_failedWords.remove(word.toLower());
    }
}

void WordEngine::clearWrongWords()
{
    m_wrongKeys.clear();
}

void WordEngine::clearProgress()
{
    m_masteredWords.clear();
    m_failedWords.clear();
}

QVector<WordEntry> WordEngine::wrongWords() const
{
    QVector<WordEntry> result;
    for (const auto &entry : m_words) {
        if (m_wrongKeys.contains(keyFor(entry))) {
            result.push_back(entry);
        }
    }
    return result;
}

QString WordEngine::keyFor(const WordEntry &entry)
{
    return entry.word.toLower() + QStringLiteral("|") + entry.pos.toLower() + QStringLiteral("|") + entry.meaning;
}

QString WordEngine::serialize(const WordEntry &entry)
{
    auto clean = [](QString value) {
        value.replace('\n', ' ');
        value.replace('\r', ' ');
        return value.trimmed();
    };
    return QStringLiteral("%1|%2|%3|%4")
        .arg(clean(entry.word), clean(entry.pos), clean(entry.meaning), clean(entry.example));
}
