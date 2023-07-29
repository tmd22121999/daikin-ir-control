#include <Arduino.h>
#define DISABLE_CODE_FOR_RECEIVER // Disables restarting receiver after each send. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not used.
#include "PinDefinitionsAndMore.h" // Define macros for input and output pin etc.


const int bg  = 4;
int temp =23;
int a=2;

void setup() { 
    
    
    TCNT2 = 0;
    TCCR2A |= _BV(COM2B1); 
    const uint16_t tPWMWrapValue = (F_CPU / 2000) / (38000); // 210,52 for 38 kHz @16 MHz clock, 2000 instead of 1000 because of Phase Correct PWM
    TCCR2A = _BV(WGM20); // PWM, Phase Correct, Top is OCR2A
    TCCR2B = _BV(WGM22) | _BV(CS20); // CS20 -> no prescaling
    OCR2A = tPWMWrapValue - 1; // The top value for the timer.  The modulation frequency will be F_CPU / 2 / OCR2A.
    OCR2B = ((tPWMWrapValue * 30) / 100) - 1;
    TCNT2 = 0;
    
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(bg, INPUT);
   
#if defined(_AVR_ATmega32U4_) || defined(SERIAL_PORT_USBVIRTUAL) || defined(SERIAL_USB) /*stm32duino*/|| defined(USBCON) /*STM32_stm32*/|| defined(SERIALUSB_PID) || defined(ARDUINO_attiny3217)
    delay(4000); // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!
#endif

}


int timer;
int modeCounter = 0;

void wakeUp() {
  timer = 0;
}

unsigned long aMicroseconds;
void loop() {
  

    
    
     timer = 0;
     attachInterrupt(0, wakeUp, HIGH);

    const uint16_t rawData[] = {
5138, 2004,  422, 1678,  422, 584,  424, 584,  422, 586,  422, 1676,  422, 584,  424, 584,  422, 584,  424, 586,  422, 1676,  424, 584,  422, 1676,  424, 1676, 424, 584,  422, 1676,  422, 1676,  424, 1676,  422, 1676,  422, 1676,  422, 586,  422, 586,  422, 1678,  422, 584,  422, 584,  422, 584,  422, 584,  424, 584,  424, 584,  422, 1676,  422, 1678,  420, 1678,  420, 1676,  422, 1678,  422, 588,  420, 1678,  420, 1678,  420, 586,  422, 588,  418, 590,  418, 588,  420, 588,  394, 614,  394, 612,  396, 612,  396, 612,  392, 614,  394, 616,  394, 612,  394, 1704,  390, 1708,  390, 1708,  390, 1710,  388, 618,  390, 618,  390, 618,  390, 616,  390, 28244,  5104, 2036,  390, 1708,  390, 618,  390, 618,  392, 616,  392, 1708,  392, 616,  392, 616,  392, 616,  392, 616,  392, 1708,  390, 618,  392, 1706,  390, 1708,  390, 618,  392, 1706,  392, 1706,  392, 1708,  392, 1706,  392, 1708,  392, 616,  392, 616,  392, 1706,  392, 616,  394, 614,  392, 614,  392, 616,  390, 616,  392, 614,  394, 612,  392, 616,  392, 616,  390, 616,  392, 1708,  390, 1710,  390, 618,  392, 616,  390, 1710,  390, 616,  392, 1708,  390, 1708,  390, 1708,  390, 616,  392, 616,  390, 618,  390, 1708,  390, 1710,  388, 618,  390, 618,  390, 1710,  388, 620,  388, 618,  388, 620,  388, 1712,  388, 1712,  386, 1712,  362, 1738,  362, 644,  364, 644,  364, 644,  362, 644,  362, 644,  364, 644,  362, 644,  362, 646,  362, 646,  362, 646,  362, 646,  364, 644,  360, 646,  362, 646,  362, 646,  360, 646,  360, 646,  360, 1738,  386, 622,  360, 646,  360, 648,  360, 1738,  384, 624,  360, 646,  384, 622,  360, 1738,  384, 624,  384, 1714,  384, 626,  382, 624,  384, 622,  384, 624,  384, 624,  384, 624,  382, 624,  382, 626,  384, 624,  382, 626,  382, 624,  382, 626,  384, 1716,  382, 1716,  382, 624,  384, 622,  384, 1714,  386, 1714,  384, 622,  384, 624,  384}; 
  
  while (timer < 10000) {
    if (digitalRead(bg) == HIGH) {
      temp--; 
      if(temp<20) {temp=25;}
      int test= 2*temp -20;
      int checks= 2*(temp -20)+37;
      timer = 0;
      delay(500);
      int dem;
      // Gửi raw data dau tien
      for (uint_fast16_t i = 0; i < 262; i++) {
        aMicroseconds = rawData[i];
        sendDataTest(i,aMicroseconds);
      }
      // Gui raw data cho nhiet do
      for (uint_fast16_t i = 262; i < 278; i++) {
        if(i%2==0){
          dem = (i-262)/2;

          if(bitRead(test,dem)!=1){
            sendDataTest(i,360);
            sendDataTest(i+1,646);
          }else{
            sendDataTest(i,360);
            sendDataTest(i+1,1738);
          }
        }
      }
      for (uint_fast16_t i = 278; i < 310; i++) {
        aMicroseconds = rawData[i];
        sendDataTest(i,aMicroseconds);
      }
      // gui raw data cho du lieu checksum
      for (uint_fast16_t i = 310; i < 326; i++) {
        if(i%2==0){
          dem = (i-310)/2;
          if(bitRead(checks,dem)!=1){
            sendDataTest(i,380);
            sendDataTest(i+1,620);
          }else{
            sendDataTest(i,380);
            sendDataTest(i+1,1716);
          }
        }
      }
      sendDataTest(326,380);   
    }
  
    delay(1);
    timer = timer + 1;



  }  
}


void sendDataTest(uint_fast16_t i,uint16_t aMicroseconds){

  
        if (i & 1) {
            unsigned long start = micros() - (64 / clockCyclesPerMicrosecond());
            while (micros() - start < aMicroseconds) {    }
          } else {
           analogWrite(3, ((256L * 30) / 100));
            unsigned long start = micros() - (64 / clockCyclesPerMicrosecond());
            while (micros() - start < aMicroseconds) {  }
           analogWrite(3, 0);
          }
  
}
