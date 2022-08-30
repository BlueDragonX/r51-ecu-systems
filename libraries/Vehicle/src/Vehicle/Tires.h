#ifndef _R51_VEHICLE_TIRES_H
#define _R51_VEHICLE_TIRES_H

#include <Arduino.h>
#include <Canny.h>
#include <Caster.h>
#include <Common.h>

namespace R51 {

enum class TireEvent : uint8_t {
    PRESSURE_STATE = 0x00,
    REQUEST = 0x01,
    SWAP_POSITION = 0x02,
};

// Track tire pressure as reported in the 0x385 CAN frame.
class TirePressureState : public Caster::Node<Message> {
    public:
        TirePressureState(uint32_t tick_ms = 0, Faker::Clock* clock = Faker::Clock::real());

        // Handle 0x385 tire pressure state frames. Returns true if the state
        // changed as a result of handling the frame.
        void handle(const Message& msg) override;

        // Yield a TIRE_PRESSURE_STATE frame on change or tick.
        void emit(const Caster::Yield<Message>& yield) override;

    private:
        bool changed_;
        Event event_;
        Ticker ticker_;
        uint8_t map_[4];

        void handleFrame(const Canny::Frame& frame);
        void handleEvent(const Event& event);
};

}

#endif  // _R51_VEHICLE_TIRES_H