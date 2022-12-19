#ifndef _R51_CTRL_PICO_H_
#define _R51_CTRL_PICO_H_

#include <Arduino.h>
#include <CRC32.h>
#include <Core.h>
#include "Config.h"

extern "C" {
    #include <hardware/flash.h>
    #include <hardware/sync.h>
};

namespace R51 {

class PicoFilteredPipe : public PicoPipe {
    public:
        PicoFilteredPipe() : PicoPipe(IO_CORE_BUFFER_SIZE, PROC_CORE_BUFFER_SIZE) {}

        // Filtering for the I/O core. Forwards all frames  to the processing core.
        bool filterLeft(const Message&) override { return true; }

        // Filtering for the processing core. Only forwards CAN bus frames and
        // system events to the I/O core.
        bool filterRight(const Message& msg) override {
            return msg.type() == Message::CAN_FRAME ||
                msg.type() == Message::J1939_MESSAGE;
        }

        void onBufferOverrun(const Message& msg) override {
            DEBUG_MSG_OBJ("pipe: dropped frame: ", msg);
        }
};

}  // namespace R51

#endif  // _R51_CTRL_PICO_H_
