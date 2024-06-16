/*

*/

/****************************************************************
****************************************************************/
#include <Dahua_DEE1010B_S2.h>

/****************************************************************
 *FUNCTION NAME:
 *FUNCTION     :
 *INPUT        :
 *OUTPUT       :
 ****************************************************************/
#if defined(ESP32)
void Dahua_DEE1010B_S2::begin(HardwareSerial *port, uint8_t relay_pin, uint8_t button_pin)
{
	_port = port;
	_relay_pin = relay_pin;
	_buttton_pin = button_pin;
	_clear_buffer();
}
#else
void Dahua_DEE1010B_S2::begin(SoftwareSerial *port, uint8_t relay_pin, uint8_t button_pin)
{
	_port = port;
	_relay_pin = relay_pin;
	_buttton_pin = button_pin;
	_clear_buffer();
}
void Dahua_DEE1010B_S2::begin(HardwareSerial *port, uint8_t relay_pin, uint8_t button_pin)
{
	_port = port;
	_relay_pin = relay_pin;
	_buttton_pin = button_pin;
	_clear_buffer();
}
#endif

/****************************************************************
 *FUNCTION NAME:
 *FUNCTION     :
 *INPUT        :
 *OUTPUT       :
 ****************************************************************/
void Dahua_DEE1010B_S2::Iterate()
{
	unsigned long startTime = millis();
	uint8_t button_open_request = 0;
	do
	{
		if (_port->available())
		{
			_frame_byte = _port->read();
			_frame_receive_byte_index++;
			if (_frame_receive_byte_index > 13) // too long, reset
			{
				_clear_buffer();
			}

			if (_frame_receive_start_flag == 0) // waiting for frame beggining 0xFD
			{
				if (_frame_byte == 0xFD)
				{
					_frame_receive_start_flag = 1;
					_frame_receive_byte_index = 1;
					_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
					_frame_to_send[_frame_receive_byte_index - 1] = _frame_byte;
				}
			}

			else // read master frame and build slave frame
			{
				if ((_frame_receive_byte_index == 2) || (_frame_receive_byte_index == 3) || (_frame_receive_byte_index == 4))
				{
					_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
					_frame_to_send[_frame_receive_byte_index - 1] = _frame_byte;
				}
				if (_frame_receive_byte_index == 5)
				{
					if (_frame_byte == 0x11) // normal frame
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = 0x51;
						_frame_receive_type_flag = 0;
					}
					else if (_frame_byte == 0x32) // frame to open door
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = 0x51;
						_frame_receive_type_flag = 1;
					}
					else if (_frame_byte == 0x61) // start frame no1
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = 0x01;
						_frame_receive_type_flag = 3;
					}
					else if (_frame_byte == 0x51) // start frame no2
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = 0x51;
						_frame_receive_type_flag = 4;
					}
					else if (_frame_byte == 0x73) // start frame no3
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = 0x51;
						_frame_receive_type_flag = 5;
					}
					else if (_frame_byte == 0x0B) // start frame no4
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = 0x51;
						_frame_receive_type_flag = 6;
					}
					else // wrong frame, reset
					{
						_clear_buffer();
					}
				}

				if (_frame_receive_type_flag == 0)
				{
					if (_frame_receive_byte_index == 6)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = 0x01;
						_frame_to_send[_frame_receive_byte_index - 0] = 0x00;
					}

					if (_frame_receive_byte_index == 7)
					{
						if (_frame_byte == _calculate_sum(_buf_received, BUF_RECEIVE_SIZE))
						{
							_buf_received[_frame_receive_byte_index - 0] = _frame_byte;
							_frame_to_send[_frame_receive_byte_index + 0] = _calculate_sum(_frame_to_send, BUF_RESPONSE_SIZE);
							_port->write(_frame_to_send, BUF_RESPONSE_SIZE_normal);
							_clear_buffer();
						}
						else // sum not correct
						{
							_clear_buffer();
						}
					}
				}

				if (_frame_receive_type_flag == 1)
				{
					if (_frame_receive_byte_index == 6)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = 0x01;
					}
					if (_frame_receive_byte_index == 7)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = 0x00;
					}
					if (_frame_receive_byte_index == 8)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
					}
					if (_frame_receive_byte_index == 9)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
					}
					if (_frame_receive_byte_index == 10)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_time_to_open_door = _frame_byte;
						button_open_request = 1;
					}
					if (_frame_receive_byte_index == 11)
					{
						if (_frame_byte == _calculate_sum(_buf_received, BUF_RECEIVE_SIZE))
						{
							_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
							_frame_to_send[_frame_receive_byte_index - 4] = _calculate_sum(_frame_to_send, BUF_RESPONSE_SIZE);
							_port->write(_frame_to_send, BUF_RESPONSE_SIZE_normal);
							_clear_buffer();
						}
						else // sum not correct
						{
							_clear_buffer();
						}
					}
				}

				if (_frame_receive_type_flag == 3)
				{
					if (_frame_receive_byte_index == 6)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = 0x06;
					}
					if (_frame_receive_byte_index == 7)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = 0x00;
					}
					if (_frame_receive_byte_index == 8)
					{
						if (_frame_byte == _calculate_sum(_buf_received, BUF_RECEIVE_SIZE))
						{
							_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
							_frame_to_send[_frame_receive_byte_index - 1] = 0x00;
							_frame_to_send[_frame_receive_byte_index + 0] = 0x20;
							_frame_to_send[_frame_receive_byte_index + 1] = 0x90;
							_frame_to_send[_frame_receive_byte_index + 2] = 0x00;
							_frame_to_send[_frame_receive_byte_index + 3] = 0x00;
							_frame_to_send[1] = 0x00;
							_frame_to_send[2] = 0x00;
							_frame_to_send[_frame_receive_byte_index + 4] = _calculate_sum(_frame_to_send, BUF_RESPONSE_SIZE_frame_type_3);
							_port->write(_frame_to_send, BUF_RESPONSE_SIZE_frame_type_3);
							_clear_buffer();
						}
						else // sum not correct
						{
							_clear_buffer();
						}
					}
				}

				if (_frame_receive_type_flag == 4)
				{
					if (_frame_receive_byte_index == 6)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = 0x01;
					}
					if (_frame_receive_byte_index == 7)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = 0x00;
					}
					if (_frame_receive_byte_index == 8)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
					}
					if (_frame_receive_byte_index == 9)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
					}
					if (_frame_receive_byte_index == 10)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
					}
					if (_frame_receive_byte_index == 11)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
					}
					if (_frame_receive_byte_index == 12)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
					}
					if (_frame_receive_byte_index == 13)
					{
						if (_frame_byte == _calculate_sum(_buf_received, BUF_RECEIVE_SIZE))
						{
							_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
							_frame_to_send[1] = 0x00;
							_frame_to_send[2] = 0x00;
							_frame_to_send[_frame_receive_byte_index - 13 + 8 - 1] = _calculate_sum(_frame_to_send, BUF_RESPONSE_SIZE_normal);
							_port->write(_frame_to_send, BUF_RESPONSE_SIZE_normal);
							_clear_buffer();
						}
						else // sum not correct
						{
							_clear_buffer();
						}
					}
				}

				if (_frame_receive_type_flag == 5)
				{
					if (_frame_receive_byte_index == 6)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = 0x03;
					}
					if (_frame_receive_byte_index == 7)
					{
						if (_frame_byte == _calculate_sum(_buf_received, BUF_RECEIVE_SIZE))
						{
							_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
							_frame_to_send[_frame_receive_byte_index - 1] = 0x30;
							_frame_to_send[_frame_receive_byte_index + 0] = 0x10;
							_frame_to_send[_frame_receive_byte_index + 1] = 0x01;
							_frame_to_send[_frame_receive_byte_index + 2] = _calculate_sum(_frame_to_send, BUF_RESPONSE_SIZE_frame_type_5);
							_port->write(_frame_to_send, BUF_RESPONSE_SIZE_frame_type_5);
							_clear_buffer();
						}
						else // sum not correct
						{
							_clear_buffer();
						}
					}
				}

				if (_frame_receive_type_flag == 6)
				{
					if (_frame_receive_byte_index == 6)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = _frame_byte;
					}
					if (_frame_receive_byte_index == 7)
					{
						_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
						_frame_to_send[_frame_receive_byte_index - 1] = 0x00;
					}
					if (_frame_receive_byte_index == 8)
					{
						if (_frame_byte == _calculate_sum(_buf_received, BUF_RECEIVE_SIZE))
						{
							_buf_received[_frame_receive_byte_index - 1] = _frame_byte;
							_frame_to_send[_frame_receive_byte_index - 1] = _calculate_sum(_frame_to_send, BUF_RESPONSE_SIZE_normal);
							_port->write(_frame_to_send, BUF_RESPONSE_SIZE_normal);
							_clear_buffer();
						}
						else // sum not correct
						{
							_clear_buffer();
						}
					}
				}
			}
		}
	} while (micros() - startTime <= _charTimeout);
	_button_and_relay_handle(button_open_request);
}

/****************************************************************
 *FUNCTION NAME:
 *FUNCTION     :
 *INPUT        :
 *OUTPUT       :
 ****************************************************************/
void Dahua_DEE1010B_S2::_clear_buffer(void)
{
	memset(_buf_received, 0, sizeof(_buf_received));
	memset(_frame_to_send, 0, sizeof(_frame_to_send));
	_frame_receive_start_flag = 0; // if 0xFD (first byte of frame) detected then change value 0 -> 1
	_frame_receive_type_flag = 0;  // if 0 normal frame, if 1 frame with open door
	_frame_receive_byte_index = 0; // frame index, number of bytes in received frame
}

/****************************************************************
 *FUNCTION NAME:
 *FUNCTION     :
 *INPUT        :
 *OUTPUT       :
 ****************************************************************/
uint8_t Dahua_DEE1010B_S2::_calculate_sum(uint8_t *frame, uint8_t frame_len)
{
	uint8_t data_sum = 0;
	for (uint8_t x = 0; x < frame_len; x++)
	{
		data_sum = data_sum + frame[x];
	}
	return data_sum;
}

/****************************************************************
 *FUNCTION NAME:
 *FUNCTION     :
 *INPUT        :
 *OUTPUT       :
 ****************************************************************/
void Dahua_DEE1010B_S2::_button_and_relay_handle(uint8_t button_open_request)
{
	unsigned long time_now = millis();
	uint8_t button_status = digitalRead(_buttton_pin);
	if (button_open_request == 1)
	{
		_relay_time_when_opened = time_now; // activate relay
	}

	if (button_status != _buttton_value_prev_stable) // pressed
	{
		if ((time_now > (_button_debounce_time_ms + _button_time_when_press_detected)) && (_buttton_value_prev_stable != button_status) && (_buttton_value_prev_undebounced != button_status)) // first press
		{
			_button_time_when_press_detected = time_now;
			_buttton_value_prev_undebounced = button_status;
		}
		else if ((time_now < (_button_debounce_time_ms + _button_time_when_press_detected)) && (_buttton_value_prev_stable != button_status) && (_buttton_value_prev_undebounced == button_status)) // undebounced state
		{
			;
		}
		else if ((time_now > (_button_debounce_time_ms + _button_time_when_press_detected)) && (_buttton_value_prev_stable != button_status) && (_buttton_value_prev_undebounced == button_status)) // debounced state
		{
			_buttton_value_prev_stable = button_status;

			_relay_time_when_opened = time_now; // activate relay
		}
	}

	if (time_now < (_relay_time_when_opened + _time_to_open_door))
	{
		digitalWrite(_relay_pin, HIGH);
	}
	else
	{
		digitalWrite(_relay_pin, LOW);
	}
}