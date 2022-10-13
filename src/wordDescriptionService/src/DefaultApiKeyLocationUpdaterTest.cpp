#include "DefaultApiKeyLocationUpdater.h"

#include <boost/optional/optional_io.hpp>

#include "../../common/fileSystem/include/FileAccessMock.h"

#include "../../common/fileSystem/include/GetProjectPath.h"

using namespace ::testing;
using namespace glossary::wordDescriptionService;

namespace
{
const std::string filePathWithPathToFileWithApiKey{common::getProjectPath("glossary") +
                                                   "config/wordsApiKeyLocation.txt"};
const std::string exampleApiKey{"apiKey"};
const std::string apiKeyLocation{"apiKeyLocation"};
}

class DefaultApiKeyLocationUpdaterTest : public Test
{
public:
    std::shared_ptr<common::FileAccessMock> fileAccess =
        std::make_shared<StrictMock<common::FileAccessMock>>();
    DefaultApiKeyLocationUpdater apiKeyLocationUpdater{fileAccess};
};

TEST_F(DefaultApiKeyLocationUpdaterTest, configFileWithPathToFileWithApiKeyNotExists_shouldDoNothing)
{
    EXPECT_CALL(*fileAccess, exists(filePathWithPathToFileWithApiKey)).WillOnce(Return(false));

    apiKeyLocationUpdater.updateApiKeyLocation(apiKeyLocation);
}

TEST_F(DefaultApiKeyLocationUpdaterTest,
       configFileWithPathToFileWithApiKeyExists_shouldOverwriteContentOfFileWithWordsApiKeyLocation)
{
    EXPECT_CALL(*fileAccess, exists(filePathWithPathToFileWithApiKey)).WillOnce(Return(true));
    EXPECT_CALL(*fileAccess, write(filePathWithPathToFileWithApiKey, apiKeyLocation));

    apiKeyLocationUpdater.updateApiKeyLocation(apiKeyLocation);
}
