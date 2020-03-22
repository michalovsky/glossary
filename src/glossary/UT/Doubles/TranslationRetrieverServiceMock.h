#pragma once

#include "gmock/gmock.h"

#include "TranslationRetrieverService.h"

class TranslationRetrieverServiceMock : public TranslationRetrieverService
{
public:
    MOCK_METHOD3(retrieveTranslation,
                 boost::optional<translator::TranslatedText>(const translator::SourceText&, translator::SourceLanguage, translator::TargetLanguage);
};
