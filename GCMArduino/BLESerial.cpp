#ifdef ARDUINO_ARCH_ARC32
#include <CurieBLE.h>
#include "BLESerial.h"
extern volatile uint32_t gu32RxSize;

uint8_t bleRxTail=0,bleRxHead=0;
uint8_t bleRxBuf[81];


BLEPeripheral blePeripheral;
BLECharacteristic serialCharact("DFB1",BLERead | BLEWrite | BLEWriteWithoutResponse | BLENotify,20);
BLECharacteristic commandAT("DFB2",BLERead | BLEWrite | BLEWriteWithoutResponse | BLENotify,20);
BLEService blunoService("DFB0");
BLEService deviceinfoService("180A"); 
BLECharacteristic modelNumberString("2A24",BLERead,"DF Bluno");
BLECharacteristic serialNumberString("2A25",BLERead,"0123456789");
BLECharacteristic firmwareRevisionString("2A26",BLERead,"FW V1.93"); 
BLECharacteristic hardwareRevisionString("2A27",BLERead,"HW V1.7");
BLECharacteristic softwareRevisionString("2A28",BLERead,"SW V1.93"); 
BLECharacteristic manufacterString("2A29",BLERead,"DFRobot");



//extern volatile unsigned char usbRxBuf[256];
//extern volatile int usbRxHead,usbRxTail;

	
#define WEAK __attribute__ ((weak))

void switchCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  char buf[20]={0};
  int length = characteristic.valueLength();
  memcpy(buf,characteristic.value(),length);
  if(length){
  //  Serial.print("len=");Serial.println(len);
  }
  if(bleRxHead){
    int i;
    for(i=0;i<bleRxTail-bleRxHead;i++){
      bleRxBuf[i] = bleRxBuf[bleRxHead+i];
    }
    bleRxTail -= bleRxHead;
    bleRxHead = 0;
  }else if(bleRxTail == 80){
      return;
  }
  if(bleRxTail+length>80){
    bleRxTail = 80;
  }else{
    memcpy((void *)&bleRxBuf[bleRxTail],buf,length);
    bleRxTail += length;
  }

}

void BLESerial::setName( const char * _name)
{
  //Serial.println(_name);
  if((strlen(_name))>20){
    memcpy(name,_name,19);
    name[19] = 0;
  }
  else{
    strcpy(name,_name);
  }
}

void BLESerial::begin(unsigned long baud)
{
	peek_buffer = -1;
	blePeripheral.setLocalName(name);
	blePeripheral.setAdvertisedServiceUuid(blunoService.uuid());
	blePeripheral.addAttribute(deviceinfoService);   // Add the BLE Battery service
	blePeripheral.addAttribute( modelNumberString );
	blePeripheral.addAttribute( serialNumberString );
    blePeripheral.addAttribute( firmwareRevisionString );
    blePeripheral.addAttribute( hardwareRevisionString );
    blePeripheral.addAttribute( softwareRevisionString );
	blePeripheral.addAttribute( manufacterString );

	blePeripheral.addAttribute(blunoService);   // Add the BLE Battery service
	blePeripheral.addAttribute(serialCharact); // add the battery level characteristic
	blePeripheral.addAttribute(commandAT);

	serialCharact.setEventHandler(BLEWritten, switchCharacteristicWritten);

	blePeripheral.begin();
}

void BLESerial::end(void)
{
}

int BLESerial::available(void)
{
  return (bleRxTail>bleRxHead);
}

int BLESerial::peek(void)
{
  if(bleRxTail-bleRxHead){
    return bleRxBuf[0];
  }else{
    return -1;
  }
}

int BLESerial::read(void)
{
  if(bleRxTail-bleRxHead){
    return bleRxBuf[bleRxHead++];
  }else{
    return -1;
  }
}

int BLESerial::availableForWrite(void)
{

  tx_buffer_index_t head = _tx_buffer_head;
  tx_buffer_index_t tail = _tx_buffer_tail;

  if (head >= tail) return SERIAL_TX_BUFFER_SIZE - 1 - head + tail;
  return tail - head - 1;
}

void BLESerial::flush(void)
{
  	if(_tx_buffer_head != _tx_buffer_tail){
		if(_tx_buffer_head > _tx_buffer_tail){
			volatile size_t sum = _tx_buffer_head - _tx_buffer_tail;
			uint8_t buf[20],len=0;
			while(sum){
				//buf[len++] = _tx_buffer[_tx_buffer_head - sum];
				buf[len++] = _tx_buffer[_tx_buffer_tail++];
				sum--;
				if((sum == 0) || (len == 20)){
					serialCharact.setValue(((const uint8_t*)buf),len);
					len = 0;
				}
				}
		}else{
		    volatile size_t sum = _tx_buffer_head + SERIAL_TX_BUFFER_SIZE - _tx_buffer_tail;
			uint8_t buf[20],len=0;	
			while(sum){
				if(_tx_buffer_tail == SERIAL_TX_BUFFER_SIZE){
					_tx_buffer_tail = (_tx_buffer_tail + 1) % SERIAL_TX_BUFFER_SIZE-1;
				}
                buf[len++] = _tx_buffer[_tx_buffer_tail++];				
				sum--;
				if((sum == 0) || (len == 20)){
					serialCharact.setValue(((const uint8_t*)buf),len);
					len = 0;
				}
			}
			
		}
	}
}

size_t BLESerial::write(uint8_t c)
{
	tx_buffer_index_t i = (_tx_buffer_head + 1) % SERIAL_TX_BUFFER_SIZE;
	 _tx_buffer[_tx_buffer_head] = c;
	 _tx_buffer_head = i;
    if(c==';'){
	 	flush();
	 	}
	 
	 return 1;
}

size_t BLESerial::write(void *buffer, size_t size)
{ 
  volatile size_t left = size;
  if (*this)  {
    while(left){
      size_t count = left>20?20:left;
      serialCharact.setValue(((const uint8_t*)buffer)+size-left,count);
      left -= count;
    }
    return size;
  }
  return size;
}

size_t BLESerial::write(const uint8_t *buffer, size_t size)
{ 
 for(size_t i=0;i<size;i++){
      write(buffer[i]);
 	}
  return 1;
}

BLESerial::operator bool() {
  bool result = false;
  BLECentral central = blePeripheral.central();
  if(central.connected())
    result = true;
  return result;
}

#endif

