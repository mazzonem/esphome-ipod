#include "click_wheel_sensor.h"
#include "esphome/core/log.h"


namespace esphome {
    namespace click_wheel_sensor {

        static const char *TAG = "click_wheel_sensor.sensor";

        float ClickWheelSensor::get_setup_priority() const {
            return setup_priority::IO;
        }

        void ClickWheelSensor::set_deadband_value(int32_t deadband_value) { this->store_.deadband_value = deadband_value; }

        void HOT ClickWheelSensorStore::gpio_intr(ClickWheelSensorStore *arg) {
            if (arg->pin_sda.digital_read()) {
                arg->cmd_packet.byte[0] |= 0x80;
            }

            if (((arg->cmd_packet.byte[0] == 128) || (arg->cmd_packet.byte[0] == 192)) && (arg->cmd_packet.byte[3] == 26)) {
                arg->cmd = 0x20;
            } else {

                arg->cmd_packet.byte[3] = (arg->cmd_packet.byte[3] >> 1);
                arg->cmd_packet.byte[3] |= (arg->cmd_packet.byte[2] << 7);

                arg->cmd_packet.byte[2] = (arg->cmd_packet.byte[2] >> 1);
                arg->cmd_packet.byte[2] |= (arg->cmd_packet.byte[1] << 7);

                arg->cmd_packet.byte[1] = (arg->cmd_packet.byte[1] >> 1);
                arg->cmd_packet.byte[1] |= (arg->cmd_packet.byte[0] << 7);

                arg->cmd_packet.byte[0] = (arg->cmd_packet.byte[0] >> 1);
            }
        }

        void ClickWheelSensor::setup() {
            ESP_LOGCONFIG(TAG, "Setting up Sensor '%s'...", this->name_.c_str());

            this->pin_sda_->setup();
            this->store_.pin_sda = this->pin_sda_->to_isr();
            this->pin_scl_->setup();
            this->store_.pin_scl = this->pin_scl_->to_isr();

            this->pin_scl_->attach_interrupt(ClickWheelSensorStore::gpio_intr, &this->store_, gpio::INTERRUPT_RISING_EDGE);
        }

        void ClickWheelSensor::loop() {
            if (this->store_.cmd == CW_CMD_FLAG) {
                if (this->store_.cmd_packet.byte[CW_BTN_CMD] != 0) {
                    this->store_.cmd = this->store_.cmd_packet.byte[2];
                    return;
                } else {
                    if (this->store_.cmd_packet.byte[CW_WHEEL_CMD] > this->store_.posn1) {
                        while (this->store_.db_compare > this->store_.deadband_value) {
                            this->store_.cmd = CW_CMD_CW;
                            this->store_.db_compare = this->store_.deadband_value / 2;
                            break;
                        }
                        this->store_.db_compare += 1;
                    }
                    if (this->store_.cmd_packet.byte[CW_WHEEL_CMD] < this->store_.posn1) {
                        while (this->store_.db_compare <= 0) {
                            this->store_.cmd = CW_CMD_CCW;
                            this->store_.db_compare = this->store_.deadband_value / 2;
                            break;
                        }
                        this->store_.db_compare -= 1;
                    }
                    this->store_.posn1 = this->store_.cmd_packet.byte[CW_WHEEL_CMD];
                }
                this->store_.cmd &= ~CW_CMD_FLAG;
                this->store_.cmd_packet.word = 0x00000000;
            }

            switch (this->store_.cmd) {
                case CW_CMD_CW:
                    this->on_clockwise_callback_.call();
                    uireset();
                    break;
                case CW_CMD_CCW:
                    this->on_anticlockwise_callback_.call();
                    uireset();
                    break;
                case CW_CMD_PP:
                    this->on_play_callback_.call();
                    uireset();
                    break;
                case CW_CMD_MENU:
                    this->on_menu_callback_.call();
                    uireset();
                    break;
                case CW_CMD_BACK:
                    this->on_backward_callback_.call();
                    uireset();
                    break;
                case CW_CMD_FORWARD:
                    this->on_forward_callback_.call();
                    uireset();
                    break;
                case CW_CMD_CBTN:
                    this->on_center_callback_.call();
                    uireset();
                    break;
            }

        }

        void ClickWheelSensor::uireset() {
            this->store_.cmd_packet.word = 0x00000000;
            this->store_.cmd = 0;
        }

        void ClickWheelSensor::dump_config() {
            LOG_SENSOR("", "ClickWheel Sensor", this);
            LOG_PIN("  Pin SDA: ", this->pin_sda_);
            LOG_PIN("  Pin SCL: ", this->pin_scl_);
            ESP_LOGCONFIG(TAG, "  Deadband Value: %d", this->store_.deadband_value);
        }

    }  // namespace click_wheel_sensor
}  // namespace esphome