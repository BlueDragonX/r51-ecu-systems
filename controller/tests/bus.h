#ifndef __R51_TESTS_BUS__
#define __R51_TESTS_BUS__

#include "src/bus.h"


class MockNode : public Node {
    public:
        MockNode(int buffer_size) :
            send_(new Frame*[buffer_size]),
            send_size_(buffer_size),
            send_count_(0) {
            for (int i = 0; i < send_size_; i++) {
                send_[i] = nullptr;
            }
        }

        Frame* recieve_ = nullptr;
        Frame** send_;
        int send_size_;
        int send_count_;
        int64_t filter1_ = 0;
        uint32_t filter2_ = 0;

        ~MockNode() {
            if (recieve_ != nullptr) {
                delete recieve_;
            }
            for (int i = 0; i < send_size_; i++) {
                if (send_[i] != nullptr) {
                    delete send_[i];
                }
            }
            delete send_;
        }

        bool recieve(Frame* frame) override {
            if (recieve_ == nullptr) {
                return false;
            }

            frame->id = recieve_->id;
            frame->len = recieve_->len;
            memcpy(frame->data, recieve_->data, recieve_->len);
            return true;
        }

        void send(Frame* frame) override {
            if (send_count_ < send_size_) {
                send_[send_count_] = new Frame();
                send_[send_count_]->id = frame->id;
                send_[send_count_]->len = frame->len;
                memcpy(send_[send_count_]->data, frame->data, frame->len);
            }
            ++send_count_;
        }

        bool filter(uint32_t id) override {
            return filter1_ == -1 || filter1_ == id || filter2_ == id;
        }
};

test(Bus, SingleBroadcast) {
    MockNode n1 = MockNode(1);
    n1.recieve_ = new Frame();
    n1.recieve_->id = 1;
    n1.recieve_->len = 2;
    n1.recieve_->data[0] = 0x11;
    n1.recieve_->data[1] = 0x22;

    MockNode n2 = MockNode(1);
    n2.recieve_ = nullptr;
    n2.filter1_ = 1;

    MockNode n3 = MockNode(1);
    n3.recieve_ = nullptr;
    n3.filter1_ = 1;

    Node* nodes[] = {&n1, &n2, &n3};

    Bus bus = Bus(nodes, sizeof(nodes)/sizeof(nodes[0]));
    bus.loop();

    assertEqual(n1.send_count_, 0);
    assertEqual(n2.send_count_, 1);
    assertTrue(framesEqual(n1.recieve_, n2.send_[0]));
    assertEqual(n3.send_count_, 1);
    assertTrue(framesEqual(n1.recieve_, n3.send_[0]));
}

test(Bus, MultiBroadcast) {
    MockNode n1 = MockNode(1);
    n1.recieve_ = new Frame();
    n1.recieve_->id = 1;
    n1.recieve_->len = 2;
    n1.recieve_->data[0] = 0x11;
    n1.recieve_->data[1] = 0x22;
    n1.filter1_ = 2;

    MockNode n2 = MockNode(1);
    n2.recieve_ = new Frame();
    n2.recieve_->id = 2;
    n2.recieve_->len = 2;
    n2.recieve_->data[0] = 0x33;
    n2.recieve_->data[1] = 0x44;
    n2.filter1_ = 1;

    MockNode n3 = MockNode(2);
    n3.recieve_ = nullptr;
    n3.filter1_ = 1;
    n3.filter2_ = 2;

    Node* nodes[] = {&n1, &n2, &n3};

    Bus bus = Bus(nodes, sizeof(nodes)/sizeof(nodes[0]));
    bus.loop();

    assertEqual(n1.send_count_, 1);
    assertTrue(framesEqual(n2.recieve_, n1.send_[0]));
    assertEqual(n2.send_count_, 1);
    assertTrue(framesEqual(n1.recieve_, n2.send_[0]));
    assertEqual(n3.send_count_, 2);
    assertTrue(framesEqual(n1.recieve_, n3.send_[0]));
    assertTrue(framesEqual(n2.recieve_, n3.send_[1]));
}

test(Bus, MultiLoop) {
    MockNode n1 = MockNode(1);
    n1.recieve_ = new Frame();
    n1.recieve_->id = 1;
    n1.recieve_->len = 2;
    n1.recieve_->data[0] = 0x11;
    n1.recieve_->data[1] = 0x22;

    MockNode n2 = MockNode(1);
    n2.recieve_ = nullptr;

    MockNode n3 = MockNode(3);
    n3.recieve_ = nullptr;
    n3.filter1_ = 1;
    n3.filter2_ = 2;

    Node* nodes[] = {&n1, &n2, &n3};

    Bus bus = Bus(nodes, sizeof(nodes)/sizeof(nodes[0]));
    bus.loop();

    assertEqual(n1.send_count_, 0);
    assertEqual(n2.send_count_, 0);
    assertEqual(n3.send_count_, 1);
    assertTrue(framesEqual(n1.recieve_, n3.send_[0]));

    n2.recieve_ = new Frame();
    n2.recieve_->id = 2;
    n2.recieve_->len = 2;
    n2.recieve_->data[0] = 0x33;
    n2.recieve_->data[1] = 0x44;

    bus.loop();

    assertEqual(n1.send_count_, 0);
    assertEqual(n2.send_count_, 0);
    assertEqual(n3.send_count_, 3);
    assertTrue(framesEqual(n1.recieve_, n3.send_[0]));
    assertTrue(framesEqual(n1.recieve_, n3.send_[1]));
    assertTrue(framesEqual(n2.recieve_, n3.send_[2]));

}

#endif  // __R51_TESTS_BUS__
