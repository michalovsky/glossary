#include "DefaultDictionaryNamesRetriever.h"

#include "gtest/gtest.h"

#include "repository/DictionaryRepositoryMock.h"

using namespace ::testing;
using namespace glossary::dictionary;

namespace
{
const std::string dictionaryName1{"dictionaryName1"};
const std::string dictionaryName2{"dictionaryName2"};
const std::string dictionaryName3{"dictionaryName3"};
const std::string dictionaryName4{"dictionaryName4"};
const Dictionary dictionary1{dictionaryName1, {}};
const Dictionary dictionary2{dictionaryName2, {}};
const std::vector<Dictionary> dictionaries1{dictionary1, dictionary2};
const std::vector<std::string> dictionaryNames1{dictionaryName1, dictionaryName2};
const std::string englishWord1{"englishWord1"};
const std::string englishWord2{"englishWord2"};
const std::string englishWord3{"englishWord3"};
const std::string englishWord4{"englishWord4"};
const std::string englishWordTranslation1{"translation1"};
const std::string englishWordTranslation2{"translation2"};
const DictionaryWord dictionaryWord1{englishWord1, englishWordTranslation1};
const DictionaryWord dictionaryWord2{englishWord2, englishWordTranslation2};
const std::vector<DictionaryWord> dictionaryWords1{dictionaryWord1, dictionaryWord2};
const std::vector<DictionaryWord> dictionaryWords2{dictionaryWord1, dictionaryWord2};
const Dictionary dictionary3{dictionaryName3, dictionaryWords1};
const Dictionary dictionary4{dictionaryName4, dictionaryWords2};
const std::vector<Dictionary> dictionaries2{dictionary1, dictionary2, dictionary3};
const std::vector<Dictionary> dictionaries3{dictionary1, dictionary3, dictionary4};
const std::vector<std::string> dictionaryNames2{dictionaryName3};
const std::vector<std::string> dictionaryNames3{dictionaryName3, dictionaryName4};
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
    EXPECT_CALL(*dictionaryRepository, getDictionaries()).WillOnce(Return(dictionaries1));

    const auto actualDictionaryNames = retriever.retrieveDictionaryNames();

    ASSERT_EQ(actualDictionaryNames, dictionaryNames1);
}

TEST_F(DefaultDictionaryNamesRetrieverTest, shouldReturnDictionaryNamesFromDictionariesContainingEnglishWord)
{
    EXPECT_CALL(*dictionaryRepository, getDictionaries()).WillOnce(Return(dictionaries2));

    const auto actualDictionaryNames = retriever.retrieveDictionaryNamesContainingEnglishWord(englishWord1);

    ASSERT_EQ(actualDictionaryNames, dictionaryNames2);
}

TEST_F(DefaultDictionaryNamesRetrieverTest, shouldReturnDictionaryNamesFromDictionariesContainingEnglishWordTranslation)
{
    EXPECT_CALL(*dictionaryRepository, getDictionaries()).WillOnce(Return(dictionaries3));

    const auto actualDictionaryNames =
        retriever.retrieveDictionaryNamesContainingEnglishWordTranslation(englishWordTranslation2);

    ASSERT_EQ(actualDictionaryNames, dictionaryNames3);
}
