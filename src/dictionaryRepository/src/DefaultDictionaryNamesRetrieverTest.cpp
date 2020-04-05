#include "gtest/gtest.h"

#include "dictionaryRepository/src/repository/DictionaryRepositoryMock.h"

#include "DefaultDictionaryNamesRetriever.h"

using namespace ::testing;
using namespace glossary::dictionaryService;

namespace
{
const DictionaryName dictionaryName1{"dictionaryName1"};
const DictionaryName dictionaryName2{"dictionaryName2"};
const Dictionary dictionary1{dictionaryName1, {}};
const Dictionary dictionary2{dictionaryName2, {}};
const Dictionaries dictionaries{dictionary1, dictionary2};
const DictionaryNames dictionaryNames{dictionaryName1, dictionaryName2};
}

class DefaultDictionaryNamesRetrieverTest : public Test
{
public:
    std::shared_ptr<DictionaryRepositoryMock> dictionaryRepository =
        std::make_shared<StrictMock<DictionaryRepositoryMock>>();
    DefaultDictionaryNamesRetriever retriever{dictionaryRepository};
};

TEST_F(DefaultDictionaryNamesRetrieverTest, shouldReturnDictionaryNames)
{
    EXPECT_CALL(*dictionaryRepository, getDictionaries()).WillOnce(Return(dictionaries));

    const auto actualDictionaryNames = retriever.retrieveDictionaryNames();

    ASSERT_EQ(actualDictionaryNames, dictionaryNames);
}