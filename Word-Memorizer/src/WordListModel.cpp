#include "WordListModel.h"

WordListModel::WordListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int WordListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_words.size();
}

QVariant WordListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_words.size()) return {};
    const WordEntry &entry = m_words.at(index.row());
    switch (role) {
    case WordRole: return entry.word;
    case PosRole: return entry.pos;
    case MeaningRole: return entry.meaning;
    case ExampleRole: return entry.example;
    default: return {};
    }
}

QHash<int, QByteArray> WordListModel::roleNames() const
{
    return {
        { WordRole, "word" },
        { PosRole, "pos" },
        { MeaningRole, "meaning" },
        { ExampleRole, "example" }
    };
}

void WordListModel::setWords(const QVector<WordEntry> &words)
{
    beginResetModel();
    m_words = words;
    endResetModel();
}
