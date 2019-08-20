#include "DatabaseImpl.h"

#include "gtest/gtest.h"
#include "FileAccessMock.h"

using namespace ::testing;

class DatabaseImplTest : public Test
{
public:
    FileAccessMock fileAccess;
    DatabaseImpl database{fileAccess};
};

TEST_F(DatabaseImplTest, givenXXX)
{

}

