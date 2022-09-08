#ifndef _R51_COMMON_CAN_
#define _R51_COMMON_CAN_

#include <Canny.h>
#include <Caster.h>

#include "Message.h"

namespace R51 {

// Bus node for reading and writing CAN frames or J1939 messages to a CAN
// controller.
template <typename Frame>
class CANNode : public Caster::Node<Message> {
    public:
        // Construct a new note that transmits frames over the given
        // connection.
        CANNode(Canny::Connection* can) : can_(can) {}
        virtual ~CANNode() = default;

        // Write a frame to the CAN bus.
        void handle(const Message& msg) override;

        // Read a single CAN frame and broadcast it to the event bus.
        void emit(const Caster::Yield<Message>& yield) override;

        // Called when a frame can't be read from the bus.
        virtual void onReadError(Canny::Error) {}

        // Called when a frame can't be written to the bus.
        virtual void onWriteError(Canny::Error, const Frame&) {}

    private:
        Canny::Connection* can_;
        Frame frame_;
};

}  // namespace R51

#include "CAN.tpp"

#endif  // _R51_COMMON_CAN_
