#ifndef _R51_BRIDGE_REALDASH_H_
#define _R51_BRIDGE_REALDASH_H_

#include <Canny.h>
#include <Caster.h>
#include <Foundation.h>
#include "Message.h"

namespace R51 {

// Caster node for communicating with RealDash. This converts Event messages to
// CAN frames which are compatible with RealDash. Can be configured to
// periodically send heartbeat to RealDash in order to keep it from timing out.
class RealDashGateway : public Caster::Node<Message> {
    public:
        // Construct a new RealDash node that communicates over the provided
        // CAN connection. Events are sent and received using the provided
        // can_id.
        //
        // If heartbeat_id is set then that a frame with that ID and a counter
        // in the first byte  is sent to RealDash every interval of
        // heartbeat_ms.
        RealDashGateway(Canny::Connection<Canny::CAN20Frame>* connection, uint32_t frame_id,
                uint32_t heartbeat_id = 0, uint32_t heartbeat_ms = 500) :
            connection_(connection), frame_id_(frame_id), hb_id_(heartbeat_id),
            hb_counter_(0), hb_ticker_(heartbeat_ms), frame_(0, 0, 8) {}

        // Encode and send an Event message to RealDash.
        void handle(const Message& msg, const Caster::Yield<Message>&) override;

        // Yield received Events from RealDash.
        void emit(const Caster::Yield<Message>& yield) override;

        // Called when a read error occurs.
        virtual void onReadError(Canny::Error) {}

        // Called when a write error occurs.
        virtual void onWriteError(Canny::Error, const Canny::CAN20Frame&) {}

    private:
        Canny::Connection<Canny::CAN20Frame>* connection_;
        uint32_t frame_id_;
        uint32_t hb_id_;
        uint8_t hb_counter_;
        Ticker hb_ticker_;
        Canny::CAN20Frame frame_;
        Event event_;
};

}  // namespace R51

#endif  // _R51_BRIDGE_REALDASH_H_
