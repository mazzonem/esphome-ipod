#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"

//clickwheel packets
#define CW_CMD_P1 3    //first byte sent in packet
#define CW_BTN_CMD 2   //second byte sent in packet
#define CW_WHEEL_CMD 1 //second byte sent in packet
#define CW_CMD_P4 0    //second byte sent in packet

//clickwheel command ‘cmd’ formatting
#define CW_CMD_CW 0x80      //clockwise scroll event (used for ‘cmd’ stuffing only)
#define CW_CMD_CCW 0x40     //counter-clockwise scroll event (used for ‘cmd’ stuffing only)
#define CW_CMD_FLAG 0x20    //clickwheel new packet arrived flag
#define CW_CMD_PP 0x08      //clickwheel play >>|| pause button pressed
#define CW_CMD_MENU 0x10    //clickwheel menu button pressed
#define CW_CMD_BACK 0x04    //clickwheel back button pressed
#define CW_CMD_FORWARD 0x02 //clickwheel forward button pressed
#define CW_CMD_CBTN 0x01    //clickwheel center button pressed

#define CW_DEADBAND 7 //adjust the sensitivity/speed of the clickwheel. lower=more sensitive

namespace esphome {
    namespace click_wheel_sensor {

        struct ClickWheelSensorStore {
            ISRInternalGPIOPin pin_sda;
            ISRInternalGPIOPin pin_scl;

            int32_t deadband_value{CW_DEADBAND};

            volatile unsigned char cmd;
            volatile char posn1, db_compare;
            volatile union
            {
                unsigned char byte[4];
                unsigned long int word;
            } cmd_packet;

            static void gpio_intr(ClickWheelSensorStore *arg);
        };

        class ClickWheelSensor : public sensor::Sensor, public Component {
        public:
            void set_pin_sda(InternalGPIOPin *pin_sda) { pin_sda_ = pin_sda; }
            void set_pin_scl(InternalGPIOPin *pin_scl) { pin_scl_ = pin_scl; }

            void set_deadband_value(int32_t deadband_value);

            void setup() override;
            void loop() override;
            void uireset();
            void dump_config() override;
            float get_setup_priority() const override;

            void add_on_clockwise_callback(std::function<void()> callback) {
                this->on_clockwise_callback_.add(std::move(callback));
            }
            void add_on_anticlockwise_callback(std::function<void()> callback) {
                this->on_anticlockwise_callback_.add(std::move(callback));
            }
            void add_on_center_callback(std::function<void()> callback) {
                this->on_center_callback_.add(std::move(callback));
            }
            void add_on_menu_callback(std::function<void()> callback) {
                this->on_menu_callback_.add(std::move(callback));
            }
            void add_on_play_callback(std::function<void()> callback) {
                this->on_play_callback_.add(std::move(callback));
            }
            void add_on_forward_callback(std::function<void()> callback) {
                this->on_forward_callback_.add(std::move(callback));
            }
            void add_on_backward_callback(std::function<void()> callback) {
                this->on_backward_callback_.add(std::move(callback));
            }

        protected:
            InternalGPIOPin *pin_sda_;
            InternalGPIOPin *pin_scl_;

            ClickWheelSensorStore store_{};

            CallbackManager<void()> on_clockwise_callback_{};
            CallbackManager<void()> on_anticlockwise_callback_{};
            CallbackManager<void()> on_center_callback_{};
            CallbackManager<void()> on_menu_callback_{};
            CallbackManager<void()> on_play_callback_{};
            CallbackManager<void()> on_forward_callback_{};
            CallbackManager<void()> on_backward_callback_{};
        };

        class ClickWheelClockwiseTrigger : public Trigger<> {
        public:
            explicit ClickWheelClockwiseTrigger(ClickWheelSensor *parent) {
                parent->add_on_clockwise_callback([this]() { this->trigger(); });
            }
        };

        class ClickWheelAnticlockwiseTrigger : public Trigger<> {
        public:
            explicit ClickWheelAnticlockwiseTrigger(ClickWheelSensor *parent) {
                parent->add_on_anticlockwise_callback([this]() { this->trigger(); });
            }
        };

        class ClickWheelCenterTrigger : public Trigger<> {
        public:
            explicit ClickWheelCenterTrigger(ClickWheelSensor *parent) {
                parent->add_on_center_callback([this]() { this->trigger(); });
            }
        };

        class ClickWheelMenuTrigger : public Trigger<> {
        public:
            explicit ClickWheelMenuTrigger(ClickWheelSensor *parent) {
                parent->add_on_menu_callback([this]() { this->trigger(); });
            }
        };

        class ClickWheelPlayTrigger : public Trigger<> {
        public:
            explicit ClickWheelPlayTrigger(ClickWheelSensor *parent) {
                parent->add_on_play_callback([this]() { this->trigger(); });
            }
        };

        class ClickWheelForwardTrigger : public Trigger<> {
        public:
            explicit ClickWheelForwardTrigger(ClickWheelSensor *parent) {
                parent->add_on_forward_callback([this]() { this->trigger(); });
            }
        };

        class ClickWheelBackwardTrigger : public Trigger<> {
        public:
            explicit ClickWheelBackwardTrigger(ClickWheelSensor *parent) {
                parent->add_on_backward_callback([this]() { this->trigger(); });
            }
        };

    }  // namespace click_wheel_sensor
}  // namespace esphome