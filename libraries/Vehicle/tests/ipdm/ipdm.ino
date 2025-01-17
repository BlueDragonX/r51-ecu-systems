#include <AUnit.h>
#include <Arduino.h>
#include <Faker.h>
#include <Test.h>
#include <Vehicle.h>

namespace R51 {

using namespace aunit;
using ::Canny::CAN20Frame;
using ::Faker::FakeClock;
using ::Faker::FakeGPIO;

test(IPDMTest, IgnoreIncorrectID) {
    FakeYield yield;
    CAN20Frame f(0x624, 0, (uint8_t[]){0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(MessageView(&f), yield);
    ipdm.emit(yield);
    assertSize(yield, 0);
}

test(IPDMTest, IgnoreIncorrectSize) {
    FakeYield yield;
    CAN20Frame f(0x625, 0, (uint8_t[]){0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(MessageView(&f), yield);
    ipdm.emit(yield);
    assertSize(yield, 0);
}

test(IPDMTest, Tick) {
    FakeClock clock;
    FakeYield yield;

    IPDM ipdm(200, &clock);
    ipdm.emit(yield);
    assertSize(yield, 0);

    Event expect((uint8_t)SubSystem::IPDM, (uint8_t)IPDMEvent::POWER_STATE, (uint8_t[]){0x00});
    clock.set(200);
    ipdm.emit(yield);
    assertSize(yield, 1);
    assertIsEvent(yield.messages()[0], expect);
}

test(IPDMTest, Defrost) {
    FakeYield yield;
    CAN20Frame f(0x625, 0, (uint8_t[]){0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(MessageView(&f), yield);
    ipdm.emit(yield);
    
    Event expect((uint8_t)SubSystem::IPDM, (uint8_t)IPDMEvent::POWER_STATE, (uint8_t[]){0x40});
    assertSize(yield, 1);
    assertIsEvent(yield.messages()[0], expect);
}

test(IPDMTest, HighBeams) {
    FakeYield yield;
    CAN20Frame f(0x625, 0, (uint8_t[]){0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(MessageView(&f), yield);
    ipdm.emit(yield);

    Event expect((uint8_t)SubSystem::IPDM, (uint8_t)IPDMEvent::POWER_STATE, (uint8_t[]){0x01});
    assertSize(yield, 1);
    assertIsEvent(yield.messages()[0], expect);
}

test(IPDMTest, LowBeams) {
    FakeYield yield;
    CAN20Frame f(0x625, 0, (uint8_t[]){0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(MessageView(&f), yield);
    ipdm.emit(yield);

    Event expect((uint8_t)SubSystem::IPDM, (uint8_t)IPDMEvent::POWER_STATE, (uint8_t[]){0x02});
    assertSize(yield, 1);
    assertIsEvent(yield.messages()[0], expect);
}

test(IPDMTest, FogLights) {
    FakeYield yield;
    CAN20Frame f(0x625, 0, (uint8_t[]){0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(MessageView(&f), yield);
    ipdm.emit(yield);

    Event expect((uint8_t)SubSystem::IPDM, (uint8_t)IPDMEvent::POWER_STATE, (uint8_t[]){0x08});
    assertSize(yield, 1);
    assertIsEvent(yield.messages()[0], expect);
}

test(IPDMTest, RunningLights) {
    FakeYield yield;
    CAN20Frame f(0x625, 0, (uint8_t[]){0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(MessageView(&f), yield);
    ipdm.emit(yield);

    Event expect((uint8_t)SubSystem::IPDM, (uint8_t)IPDMEvent::POWER_STATE, (uint8_t[]){0x04});
    assertSize(yield, 1);
    assertIsEvent(yield.messages()[0], expect);
}

test(IPDMTest, ACCompressor) {
    FakeYield yield;
    CAN20Frame f(0x625, 0, (uint8_t[]){0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

    IPDM ipdm;
    ipdm.handle(MessageView(&f), yield);
    ipdm.emit(yield);

    Event expect((uint8_t)SubSystem::IPDM, (uint8_t)IPDMEvent::POWER_STATE, (uint8_t[]){0x80});
    assertSize(yield, 1);
    assertIsEvent(yield.messages()[0], expect);
}

test(IPDMTest, RequestPowerState) {
    FakeYield yield;
    RequestCommand control(SubSystem::IPDM, (uint8_t)IPDMEvent::POWER_STATE);
    Event expect((uint8_t)SubSystem::IPDM, (uint8_t)IPDMEvent::POWER_STATE, (uint8_t[]){0x00});

    IPDM ipdm;
    ipdm.handle(MessageView(&control), yield);
    ipdm.emit(yield);
    assertSize(yield, 1);
    assertIsEvent(yield.messages()[0], expect);
}

test(IPDMTest, RequestSubSystem) {
    FakeYield yield;
    RequestCommand control(SubSystem::IPDM);
    Event expect((uint8_t)SubSystem::IPDM, (uint8_t)IPDMEvent::POWER_STATE, (uint8_t[]){0x00});

    IPDM ipdm;
    ipdm.handle(MessageView(&control), yield);
    ipdm.emit(yield);
    assertSize(yield, 1);
    assertIsEvent(yield.messages()[0], expect);
}

test(IPDMTest, RequestAll) {
    FakeYield yield;
    RequestCommand control;
    Event expect((uint8_t)SubSystem::IPDM, (uint8_t)IPDMEvent::POWER_STATE, (uint8_t[]){0x00});

    IPDM ipdm;
    ipdm.handle(MessageView(&control), yield);
    ipdm.emit(yield);
    assertSize(yield, 1);
    assertIsEvent(yield.messages()[0], expect);
}

}  // namespace R51

// Test boilerplate.
void setup() {
#ifdef ARDUINO
    delay(1000);
#endif
    SERIAL_PORT_MONITOR.begin(115200);
    while(!SERIAL_PORT_MONITOR);
}

void loop() {
    aunit::TestRunner::run();
    delay(1);
}
