#include <gmock/gmock.h>
#include <DebouncerStrategySimple.h>
#include <Debouncer.h>
using namespace std;

class DebouncerStrategySimpleTest: public testing::Test{
public:
    DebouncerStrategySimple subject;
    Debouncer debouncer;
    MOCK_METHOD0(onCallback,void());
    MOCK_METHOD0(offCallback,void());
    // Test interface
protected:
    virtual void SetUp() override;
};

void DebouncerStrategySimpleTest::SetUp(){
    subject.setOnCallback(bind(&DebouncerStrategySimpleTest::onCallback,this));
    subject.setOffCallback(bind(&DebouncerStrategySimpleTest::offCallback,this));
    debouncer.setStrategy(&subject);
    debouncer.setDebounceInterval(1ms);
}
TEST_F(DebouncerStrategySimpleTest,On){
    EXPECT_CALL(*this,onCallback()).Times(1);
    EXPECT_CALL(*this,offCallback()).Times(0);
    debouncer.on();
    this_thread::sleep_for(100ms);
}
