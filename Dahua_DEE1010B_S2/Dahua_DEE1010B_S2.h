/*

 */

#ifndef Dahua_DEE1010B_S2_h
#define Dahua_DEE1010B_S2_h

#define BUF_RECEIVE_SIZE 13
#define BUF_RESPONSE_SIZE 13
#define BUF_RESPONSE_SIZE_normal 8
#define BUF_RESPONSE_SIZE_frame_type_5 10
#define BUF_RESPONSE_SIZE_frame_type_3 13

#include "Arduino.h"
#if defined(ESP32)
#include <HardwareSerial.h>
#endif
#if defined(ESP8266)
#include <SoftwareSerial.h>
#endif
#if defined(__AVR__)
#include <SoftwareSerial.h>
#endif

class Dahua_DEE1010B_S2
{
public:
#if defined(ESP32)
	void begin(HardwareSerial *port, uint8_t relay_pin, uint8_t button_pin);
#else
	void begin(SoftwareSerial *port, uint8_t relay_pin, uint8_t button_pin);
	void begin(HardwareSerial *port, uint8_t relay_pin, uint8_t button_pin);
#endif
	void Iterate();

protected:
	Stream *_port;
	void _clear_buffer();
	uint8_t _calculate_sum(uint8_t *frame, uint8_t frame_len);
	void button_init(uint8_t pin);
	void _calculateTimeouts(unsigned long baud, uint32_t config);

	uint8_t _button_check_value(uint8_t pin);
	void _relay_init(uint8_t pin);
	void _relay_set_value(uint8_t pin);
	void _button_and_relay_handle(uint8_t button_open_request);

	uint8_t _buf_received[BUF_RECEIVE_SIZE];
	uint8_t _frame_to_send[BUF_RESPONSE_SIZE];
	uint8_t _frame_byte;
	unsigned long _charTimeout = 1040;	   // characted timeout when waiting for frame
	uint8_t _frame_receive_start_flag = 0; // if 0xFD (first byte of frame) detected then change value 0 -> 1
	uint8_t _frame_receive_type_flag = 0;  // if 0 normal frame, if 1 frame with open door, if 3,4,5,6 start frame
	uint8_t _frame_receive_byte_index = 0; // frame index, number of byte in received frame
	unsigned long _time_to_open_door = 2;  // how many ms door will be open, init value

	uint8_t _relay_pin;						   // relay pin
	unsigned long _relay_time_when_opened = 0; // time millis() when relay off -> on

	uint8_t _buttton_pin;								// button pin
	uint8_t _buttton_value_prev_stable = 1;				// button state previous stable (1 means not pressed)
	uint8_t _buttton_value_prev_undebounced = 1;		// button state previous undebounced
	unsigned long _button_time_when_press_detected = 0; // time millis() when first time button state changed
	unsigned long _button_debounce_time_ms = 40;		// 40 ms debounce time
};

#endif