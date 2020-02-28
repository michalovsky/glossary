#pragma once

#include "WordDescription.h"
#include "boost/optional.hpp"

namespace wordsDescriptionsDb
{

class WordsDescriptionsDb
{
public:
    virtual ~WordsDescriptionsDb() = default;

    virtual void addWordDescription(const WordDescription&) = 0;
    virtual boost::optional<WordDescription>
    getWordDescription(const EnglishWord&) const = 0;
    virtual WordsDescriptions getWordsDescriptions() const = 0;
    virtual bool contains(const EnglishWord&) const = 0;
};
}
