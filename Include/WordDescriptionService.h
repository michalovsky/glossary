#pragma once

#include "boost/optional.hpp"
#include "wordsDescriptionsDb/WordDescription.h"

class WordDescriptionService
{
public:
    virtual ~WordDescriptionService() = default;

    virtual boost::optional<wordsDescriptionsDb::WordDescription>
    getWordDescription(const EnglishWord&) = 0;
};