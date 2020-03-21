#pragma once

#include "boost/optional.hpp"

#include "TranslationsStorage.h"

namespace translationsDb
{
class TranslationsMemoryStorage : public TranslationsStorage
{
public:
    void addTranslation(Translation) override;
    boost::optional<Translation> getTranslation(const SourceText&) const override;
    Translations getTranslations() const override;
    bool contains(const SourceText&) const override;
    Translations::size_type size() const override;
    bool empty() const override;
    Translations::const_iterator begin() const override;
    Translations::const_iterator end() const override;

private:
    Translations::const_iterator findTranslationPosition(const SourceText&) const;

    Translations translations;
};
}
