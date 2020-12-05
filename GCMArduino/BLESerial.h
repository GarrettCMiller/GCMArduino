#ifdef ARDUINO_ARCH_ARC32
#ifndef _BLE_SERIAL_H_INCLUDED
#define _BLE_SERIAL_H_INCLUDED

#define SERIAL_RX_BUFFER_SIZE 40
#define SERIAL_TX_BUFFER_SIZE 40

#if (SERIAL_TX_BUFFER_SIZE>256)
typedef uint16_t tx_buffer_index_t;
#else
typedef uint8_t tx_buffer_index_t;
#endif
#if  (SERIAL_RX_BUFFER_SIZE>256)
typedef uint16_t rx_buffer_index_t;
#else
typedef uint8_t rx_buffer_index_t;
#endif

#define BLE_BUFFER_SIZE 20
#include <HardwareSerial.h>


class BLESerial : public HardwareSerial
{
private:
  int peek_buffer;
  
  unsigned char _rx_buffer[SERIAL_RX_BUFFER_SIZE];
  unsigned char _tx_buffer[SERIAL_TX_BUFFER_SIZE];
  volatile rx_buffer_index_t _rx_buffer_head;
  volatile rx_buffer_index_t _rx_buffer_tail;
  volatile tx_buffer_index_t _tx_buffer_head;
  volatile tx_buffer_index_t _tx_buffer_tail;
public:
  BLESerial() { peek_buffer = -1;};
    void setName(const char * _name);
    void begin(unsigned long baud=57600);
    void end(void);

    int available(void);
	int availableForWrite(void);
    int peek(void);
    int read(void);
    void flush(void);
    virtual size_t write(uint8_t);
    virtual size_t write(void*, size_t);
    virtual size_t write(const uint8_t *, size_t);
  using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool();
public:
  char name[20];
};

//extern Serial_ BLESerial;
#endif
#endif