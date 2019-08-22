#include <gtest/gtest.h>
#include "InputPort_mock.h"
#include <functional>

using namespace std;
using testing::Return;
class InputPortTest:public testing::Test{
public:
    InputPort_mock subject;
    MOCK_METHOD1(myCb,void(bool));
};
TEST_F(InputPortTest,defaultCallback){
    subject.explicitEventCallback();//No exception
}
TEST_F(InputPortTest, registerCallback){
    subject.SetEventCallback(bind(&InputPortTest::myCb,this,placeholders::_1));
    EXPECT_CALL(*this,myCb(testing::_)).Times(1);
    subject.explicitEventCallback();
}
TEST_F(InputPortTest, unEegisterCallback){
    subject.SetEventCallback(bind(&InputPortTest::myCb,this,placeholders::_1));
    subject.RemoveEventCallback();
    EXPECT_CALL(*this,myCb).Times(0);
    subject.explicitEventCallback();
}
TEST_F(InputPortTest, passReadValueToCallback){
    subject.SetEventCallback(bind(&InputPortTest::myCb,this,placeholders::_1));
    EXPECT_CALL(subject,Read()).WillOnce(Return(true));
    EXPECT_CALL(*this,myCb(true)).Times(1);
    subject.explicitEventCallback();
    EXPECT_CALL(subject,Read()).WillOnce(Return(false));
    EXPECT_CALL(*this,myCb(false)).Times(1);
    subject.explicitEventCallback();

}
