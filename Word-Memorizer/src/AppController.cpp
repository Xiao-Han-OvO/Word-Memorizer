#include "AppController.h"

#include <QFileInfo>
#include <QVariantMap>

AppController::AppController(QObject *parent)
    : QObject(parent)
{
}

void AppController::loadWordBook(const QUrl &url)
{
    const QString fileName = WordEngine::normalizeFileUrl(url);
    QString error;
    if (!m_engine.loadFromFile(fileName, &error)) {
        setStatus(error.isEmpty() ? QStringLiteral("加载失败，请检查词库格式") : error);
        emit toastRequested(m_statusText, QStringLiteral("error"));
        return;
    }

    refreshWrongModel();
    emitAllStats();
    setStatus(QStringLiteral("已加载：%1").arg(QFileInfo(fileName).fileName()));
    emit toastRequested(QStringLiteral("成功加载 %1 个单词").arg(m_engine.totalCount()), QStringLiteral("success"));
    nextWord();
}

void AppController::nextWord()
{
    if (m_current.isValid() && m_engine.hasFailed(m_current.word) && m_inputEnabled) {
        m_engine.markWrong(m_current);
        refreshWrongModel();
    }

    if (!m_engine.hasWords()) {
        m_current = {};
        setFeedback(QString(), QStringLiteral("info"));
        setInputEnabled(false);
        setStatus(QStringLiteral("请先打开单词库开始学习"));
        emit currentWordChanged();
        emitAllStats();
        return;
    }

    if (m_engine.allMastered()) {
        setInputEnabled(false);
        setFeedback(QStringLiteral("🎉 全部单词已完成！"), QStringLiteral("success"));
        setStatus(QStringLiteral("学习完成：%1 / %2").arg(m_engine.masteredCount()).arg(m_engine.totalCount()));
        emitAllStats();
        return;
    }

    m_current = m_engine.nextWord();
    setAttemptCount(0);
    setFeedback(QString(), QStringLiteral("info"));
    setInputEnabled(m_current.isValid());
    if (m_current.isValid()) {
        if (m_engine.hasFailed(m_current.word)) {
            setStatus(QStringLiteral("这个单词你之前拼错过，请重点复习"));
        } else {
            setStatus(QStringLiteral("根据释义拼写英文单词"));
        }
    }
    emit currentWordChanged();
    emitAllStats();
}

void AppController::submitAnswer(const QString &answer)
{
    if (!m_current.isValid() || !m_inputEnabled) return;

    if (answer.trimmed().compare(m_current.word, Qt::CaseInsensitive) == 0) {
        m_engine.markMastered(m_current.word);
        setFeedback(QStringLiteral("✓ 正确！%1").arg(m_current.word), QStringLiteral("success"));
        setInputEnabled(false);
        refreshWrongModel();
        emitAllStats();
        if (m_engine.allMastered()) {
            setStatus(QStringLiteral("恭喜，你已掌握全部 %1 个单词").arg(m_engine.totalCount()));
            emit toastRequested(QStringLiteral("所有单词已完成！"), QStringLiteral("success"));
        } else {
            setStatus(QStringLiteral("答对了，点击“下一个”继续"));
        }
        return;
    }

    m_engine.markWrong(m_current);
    m_engine.markFailed(m_current.word);
    setAttemptCount(m_attemptCount + 1);
    setFeedback(QStringLiteral("✕ 还不对，再试一次"), QStringLiteral("error"));
    setStatus(QStringLiteral("已加入错词本。你可以继续尝试，或查看答案"));
    refreshWrongModel();
    emitAllStats();
}

void AppController::revealAnswer()
{
    if (!m_current.isValid() || !m_inputEnabled) return;
    m_engine.markWrong(m_current);
    m_engine.markFailed(m_current.word);
    setFeedback(QStringLiteral("正确答案：%1").arg(m_current.word), QStringLiteral("answer"));
    setInputEnabled(false);
    setStatus(QStringLiteral("已加入错词本，点击“下一个”继续"));
    refreshWrongModel();
    emitAllStats();
}

void AppController::resetProgress()
{
    m_engine.clearProgress();
    m_engine.clearWrongWords();
    refreshWrongModel();
    emitAllStats();
    setStatus(QStringLiteral("学习进度和错词本已重置"));
    emit toastRequested(QStringLiteral("已重置学习进度"), QStringLiteral("success"));
    nextWord();
}

void AppController::clearWrongWords()
{
    m_engine.clearWrongWords();
    refreshWrongModel();
    emitAllStats();
    setStatus(QStringLiteral("错词本已清空"));
    emit toastRequested(QStringLiteral("错词本已清空"), QStringLiteral("success"));
}

void AppController::exportWrongWords(const QUrl &url)
{
    if (m_engine.wrongCount() == 0) {
        emit toastRequested(QStringLiteral("错词本为空，无需导出"), QStringLiteral("info"));
        return;
    }

    QString fileName = WordEngine::normalizeFileUrl(url);
    if (!fileName.endsWith(QStringLiteral(".txt"), Qt::CaseInsensitive)) {
        fileName += QStringLiteral(".txt");
    }

    QString error;
    if (!m_engine.exportWrongWords(fileName, &error)) {
        emit toastRequested(error, QStringLiteral("error"));
        return;
    }
    setStatus(QStringLiteral("错词本已导出：%1").arg(QFileInfo(fileName).fileName()));
    emit toastRequested(QStringLiteral("错词本导出成功"), QStringLiteral("success"));
}

QVariantList AppController::loadEditorWords(const QUrl &url)
{
    const QString fileName = WordEngine::normalizeFileUrl(url);
    QString error;
    const QVector<WordEntry> entries = WordEngine::readWordFile(fileName, &error);
    if (entries.isEmpty()) {
        emit toastRequested(error.isEmpty() ? QStringLiteral("没有读取到单词") : error, QStringLiteral("error"));
        return {};
    }

    QVariantList list;
    for (const auto &entry : entries) list << wordToVariant(entry);
    emit toastRequested(QStringLiteral("已导入 %1 个单词到编辑器").arg(list.size()), QStringLiteral("success"));
    return list;
}

bool AppController::saveEditorWords(const QUrl &url, const QVariantList &items)
{
    QVector<WordEntry> entries = variantsToWords(items);
    if (entries.isEmpty()) {
        emit toastRequested(QStringLiteral("没有可保存的单词"), QStringLiteral("error"));
        return false;
    }

    QString fileName = WordEngine::normalizeFileUrl(url);
    if (!fileName.endsWith(QStringLiteral(".txt"), Qt::CaseInsensitive)) {
        fileName += QStringLiteral(".txt");
    }

    QString error;
    if (!m_engine.saveToFile(fileName, entries, &error)) {
        emit toastRequested(error, QStringLiteral("error"));
        return false;
    }
    emit toastRequested(QStringLiteral("词库已保存：%1").arg(QFileInfo(fileName).fileName()), QStringLiteral("success"));
    return true;
}

QString AppController::debugInfo() const
{
    return QStringLiteral("总单词数：%1\n已掌握：%2\n错词数量：%3\n剩余：%4\n当前单词：%5\n输入状态：%6")
        .arg(m_engine.totalCount())
        .arg(m_engine.masteredCount())
        .arg(m_engine.wrongCount())
        .arg(m_engine.remainingCount())
        .arg(m_current.word.isEmpty() ? QStringLiteral("无") : m_current.word)
        .arg(m_inputEnabled ? QStringLiteral("可输入") : QStringLiteral("锁定"));
}

void AppController::setFeedback(const QString &text, const QString &kind)
{
    if (m_feedbackText == text && m_feedbackKind == kind) return;
    m_feedbackText = text;
    m_feedbackKind = kind;
    emit feedbackChanged();
}

void AppController::setStatus(const QString &text)
{
    if (m_statusText == text) return;
    m_statusText = text;
    emit statusChanged();
}

void AppController::setInputEnabled(bool enabled)
{
    if (m_inputEnabled == enabled) return;
    m_inputEnabled = enabled;
    emit inputStateChanged();
}

void AppController::setAttemptCount(int count)
{
    if (m_attemptCount == count) return;
    m_attemptCount = count;
    emit attemptChanged();
}

void AppController::refreshWrongModel()
{
    m_wrongWordsModel.setWords(m_engine.wrongWords());
}

void AppController::emitAllStats()
{
    emit statsChanged();
}

QVector<WordEntry> AppController::variantsToWords(const QVariantList &items)
{
    QVector<WordEntry> entries;
    for (const QVariant &item : items) {
        const QVariantMap map = item.toMap();
        WordEntry entry;
        entry.word = map.value(QStringLiteral("word")).toString().trimmed();
        entry.pos = map.value(QStringLiteral("pos")).toString().trimmed();
        entry.meaning = map.value(QStringLiteral("meaning")).toString().trimmed();
        entry.example = map.value(QStringLiteral("example")).toString().trimmed();
        if (entry.isValid()) entries.push_back(entry);
    }
    return entries;
}

QVariantMap AppController::wordToVariant(const WordEntry &entry)
{
    return {
        { QStringLiteral("word"), entry.word },
        { QStringLiteral("pos"), entry.pos },
        { QStringLiteral("meaning"), entry.meaning },
        { QStringLiteral("example"), entry.example }
    };
}
