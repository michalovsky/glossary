#pragma once

#include <QList>
#include <QString>

namespace glossary::gui::view
{
struct FormattedDictionaryWord
{
    QString englishWord;
    QString translation;
};

using FormattedDictionaryWords = QList<FormattedDictionaryWord>;

inline QStringList getAccumulatedFormattedDictionaryWords(const FormattedDictionaryWords& dictionaryWords)
{
    QStringList accumulatedDictionaryWords;
    for (const auto& dictionaryWord : dictionaryWords)
    {
        auto accumulatedDictionaryWord = dictionaryWord.englishWord + " - " + dictionaryWord.translation;
        accumulatedDictionaryWords.push_back(accumulatedDictionaryWord);
    }
    return accumulatedDictionaryWords;
}

inline bool operator==(const FormattedDictionaryWord& lhs, const FormattedDictionaryWord& rhs)
{
    return lhs.englishWord == rhs.englishWord && lhs.translation == rhs.translation;
}
}
