#pragma once

#include "WordDescriptionRepositoryFactory.h"

namespace glossary::wordDescriptionRepository
{
class DefaultWordDescriptionRepositoryFactory : public WordDescriptionRepositoryFactory
{
public:
    explicit DefaultWordDescriptionRepositoryFactory(std::shared_ptr<const common::FileAccess>);

    std::unique_ptr<WordDescriptionRepository> createWordDescriptionRepository() const override;

private:
    std::shared_ptr<const common::FileAccess> fileAccess;
};
}