#pragma once

#include "WordEngine.h"
#include "WordListModel.h"

#include <QObject>
#include <QUrl>
#include <QVariantList>

class AppController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString currentWord READ currentWord NOTIFY currentWordChanged)
    Q_PROPERTY(QString currentPos READ currentPos NOTIFY currentWordChanged)
    Q_PROPERTY(QString currentMeaning READ currentMeaning NOTIFY currentWordChanged)
    Q_PROPERTY(QString currentExample READ currentExample NOTIFY currentWordChanged)
    Q_PROPERTY(QString feedbackText READ feedbackText NOTIFY feedbackChanged)
    Q_PROPERTY(QString feedbackKind READ feedbackKind NOTIFY feedbackChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusChanged)
    Q_PROPERTY(bool loaded READ loaded NOTIFY statsChanged)
    Q_PROPERTY(bool completed READ completed NOTIFY statsChanged)
    Q_PROPERTY(bool inputEnabled READ inputEnabled NOTIFY inputStateChanged)
    Q_PROPERTY(int attemptCount READ attemptCount NOTIFY attemptChanged)
    Q_PROPERTY(int totalCount READ totalCount NOTIFY statsChanged)
    Q_PROPERTY(int masteredCount READ masteredCount NOTIFY statsChanged)
    Q_PROPERTY(int wrongCount READ wrongCount NOTIFY statsChanged)
    Q_PROPERTY(int remainingCount READ remainingCount NOTIFY statsChanged)
    Q_PROPERTY(double progress READ progress NOTIFY statsChanged)
    Q_PROPERTY(WordListModel* wrongWordsModel READ wrongWordsModel CONSTANT)

public:
    explicit AppController(QObject *parent = nullptr);

    QString currentWord() const { return m_current.word; }
    QString currentPos() const { return m_current.pos; }
    QString currentMeaning() const { return m_current.meaning; }
    QString currentExample() const { return m_current.example; }
    QString feedbackText() const { return m_feedbackText; }
    QString feedbackKind() const { return m_feedbackKind; }
    QString statusText() const { return m_statusText; }
    bool loaded() const { return m_engine.hasWords(); }
    bool completed() const { return m_engine.allMastered(); }
    bool inputEnabled() const { return m_inputEnabled; }
    int attemptCount() const { return m_attemptCount; }
    int totalCount() const { return m_engine.totalCount(); }
    int masteredCount() const { return m_engine.masteredCount(); }
    int wrongCount() const { return m_engine.wrongCount(); }
    int remainingCount() const { return m_engine.remainingCount(); }
    double progress() const { return m_engine.progress(); }
    WordListModel* wrongWordsModel() { return &m_wrongWordsModel; }

    Q_INVOKABLE void loadWordBook(const QUrl &url);
    Q_INVOKABLE void nextWord();
    Q_INVOKABLE void submitAnswer(const QString &answer);
    Q_INVOKABLE void revealAnswer();
    Q_INVOKABLE void resetProgress();
    Q_INVOKABLE void clearWrongWords();
    Q_INVOKABLE void exportWrongWords(const QUrl &url);
    Q_INVOKABLE QVariantList loadEditorWords(const QUrl &url);
    Q_INVOKABLE bool saveEditorWords(const QUrl &url, const QVariantList &items);
    Q_INVOKABLE QString debugInfo() const;

signals:
    void currentWordChanged();
    void feedbackChanged();
    void statusChanged();
    void inputStateChanged();
    void attemptChanged();
    void statsChanged();
    void toastRequested(const QString &message, const QString &kind);

private:
    void setFeedback(const QString &text, const QString &kind);
    void setStatus(const QString &text);
    void setInputEnabled(bool enabled);
    void setAttemptCount(int count);
    void refreshWrongModel();
    void emitAllStats();
    static QVector<WordEntry> variantsToWords(const QVariantList &items);
    static QVariantMap wordToVariant(const WordEntry &entry);

    WordEngine m_engine;
    WordEntry m_current;
    WordListModel m_wrongWordsModel;
    QString m_feedbackText;
    QString m_feedbackKind = QStringLiteral("info");
    QString m_statusText = QStringLiteral("请先打开单词库开始学习");
    int m_attemptCount = 0;
    bool m_inputEnabled = false;
};
