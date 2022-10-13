#pragma once

#include <memory>
#include <mutex>

#include "WordDescriptionRepository.h"
#include "WordsDescriptionsStorage.h"

namespace glossary::wordDescriptionRepository
{
class DefaultWordDescriptionRepository : public WordDescriptionRepository
{
public:
    explicit DefaultWordDescriptionRepository(std::unique_ptr<WordsDescriptionsStorage>);

    void addWordDescription(const WordDescription&) override;
    boost::optional<WordDescription> getWordDescription(const std::string&) const override;
    bool contains(const std::string&) const override;

private:
    std::unique_ptr<WordsDescriptionsStorage> storage;
    mutable std::mutex lock;
};
}
