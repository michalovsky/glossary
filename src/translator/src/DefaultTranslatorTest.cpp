#include "DefaultTranslator.h"

#include <boost/optional/optional_io.hpp>

#include "gtest/gtest.h"

#include "TranslationDeserializerMock.h"
#include "TranslationRequestFormatterMock.h"
#include "httpClient/HttpClientMock.h"

#include "exceptions/InvalidApiKey.h"
#include "httpClient/exceptions/ConnectionFailed.h"

using namespace ::testing;
using namespace glossary::translator;

namespace
{
const std::string baseUrl{"https://translation.googleapis.com/language/translate/v2?"};
const std::string apiKey = {"topSecretKey"};
const std::string keyField = {"key=" + apiKey};
const auto sourceField = R"(&source=pl)";
const auto targetField = R"(&target=en)";
const std::string url = baseUrl + keyField + "&q=piwo" + sourceField + targetField;
const auto responseContent = R"({"code":200,"lang":"pl-en","text":["beer"]})";
const httpClient::HttpResponse successResponse{200, responseContent};
const httpClient::HttpResponse failureResponse{400, ""};
const std::string polishText = "piwo";
const auto englishText = "beer";
const auto sourceLanguage = Language::Polish;
const auto targetLanguage = Language::English;
const httpClient::GetPayload getPayload{url, std::nullopt};
}

class DefaultTranslatorTest : public Test
{
public:
    std::shared_ptr<httpClient::HttpClientMock> handler =
        std::make_shared<StrictMock<httpClient::HttpClientMock>>();
    std::unique_ptr<TranslationDeserializerMock> deserializerInit =
        std::make_unique<StrictMock<TranslationDeserializerMock>>();
    TranslationDeserializerMock* deserializer = deserializerInit.get();
    std::unique_ptr<TranslationRequestFormatterMock> requestFormatterInit =
        std::make_unique<StrictMock<TranslationRequestFormatterMock>>();
    TranslationRequestFormatterMock* requestFormatter = requestFormatterInit.get();
    DefaultTranslator translator{handler, std::move(deserializerInit), std::move(requestFormatterInit)};
};

TEST_F(DefaultTranslatorTest, whenConnectionFails_shouldThrowConnectionFailedException)
{
    EXPECT_CALL(*requestFormatter, getFormattedRequest(polishText, sourceLanguage, targetLanguage, apiKey))
        .WillOnce(Return(url));
    EXPECT_CALL(*handler, get(getPayload)).WillOnce(Throw(httpClient::exceptions::ConnectionFailed{""}));

    ASSERT_THROW(translator.translate(polishText, sourceLanguage, targetLanguage, apiKey),
                 httpClient::exceptions::ConnectionFailed);
}

TEST_F(DefaultTranslatorTest, givenInvalidKeyResponseFromTranslationApi_throwInvalidApiKeyException)
{
    EXPECT_CALL(*requestFormatter, getFormattedRequest(polishText, sourceLanguage, targetLanguage, apiKey))
        .WillOnce(Return(url));
    EXPECT_CALL(*handler, get(getPayload)).WillOnce(Return(failureResponse));

    ASSERT_THROW(translator.translate(polishText, sourceLanguage, targetLanguage, apiKey),
                 exceptions::InvalidApiKey);
}

TEST_F(DefaultTranslatorTest, givenSuccessResponseFromTranslationApi_shouldReturnTranslatedText)
{
    EXPECT_CALL(*requestFormatter, getFormattedRequest(polishText, sourceLanguage, targetLanguage, apiKey))
        .WillOnce(Return(url));
    EXPECT_CALL(*handler, get(getPayload)).WillOnce(Return(successResponse));
    EXPECT_CALL(*deserializer, deserialize(responseContent)).WillOnce(Return(englishText));

    const auto translation = translator.translate(polishText, sourceLanguage, targetLanguage, apiKey);

    ASSERT_EQ(*translation, englishText);
}
