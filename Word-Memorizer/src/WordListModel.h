#pragma once

#include "WordEngine.h"

#include <QAbstractListModel>

class WordListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        WordRole = Qt::UserRole + 1,
        PosRole,
        MeaningRole,
        ExampleRole
    };
    Q_ENUM(Roles)

    explicit WordListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setWords(const QVector<WordEntry> &words);
    QVector<WordEntry> words() const { return m_words; }

private:
    QVector<WordEntry> m_words;
};
