#ifndef _R51_STANDALONE_CAN_H_
#define _R51_STANDALONE_CAN_H_

#include "Config.h"

#include <Canny.h>
#include <Canny/MCP2518.h>
#include "Debug.h"

Canny::MCP2518<Canny::CAN20Frame> CAN(MCP2518_CS_PIN);

namespace R51 {

// CAN connection which filters and buffers frames; and logs errors to serial.
class CANConnection : public Canny::BufferedConnection<Canny::CAN20Frame> {
    public:
        CANConnection() :
            Canny::BufferedConnection<Canny::CAN20Frame>(
                    &CAN, VEHICLE_READ_BUFFER, VEHICLE_WRITE_BUFFER) {}

        bool begin() {
            // Initialize controller.
            if (!CAN.begin(VEHICLE_CAN_MODE)) {
                return false;
            }
            // Enable hardware filtering if not in promiscuous mode.
            if (!VEHICLE_PROMISCUOUS) {
                // Read climate frames.
                CAN.setFilter(0, 0, 0x54A, 0xFFFFFFFE);
                // Read settings frames.
                CAN.setFilter(0, 0, 0x72E, 0xFFFFFFFE);
                // Read tire frames.
                CAN.setFilter(0, 0, 0x385, 0xFFFFFFFF);
                // Read IPDM frames.
                CAN.setFilter(0, 0, 0x625, 0xFFFFFFFF);
            }
            return true;
        }

        // Log read errors to debug serial.
        void onReadError(Canny::Error err) const override {
            DEBUG_MSG_VAL("can: read error: ", err);
        }

        // Log write errors to debug serial.
        void onWriteError(Canny::Error err, const Canny::CAN20Frame& frame) const override {
            DEBUG_MSG_VAL("can: write error: ", err);
            DEBUG_MSG_OBJ("can: dropped frame: ", frame);
        }
};

}  // namespace R51

#endif  // _R51_STANDALONE_CAN_H_