#ifndef SITL_H
#define SITL_H

#include "WProgram.h"
#include "Flight_Configuration.h"



class SITL{
public:
  bool initialize();
  bool update();

  byte readESense();
  byte readIsoSense();
  Mma_Data readMMA();
  Bmp_Data readBMP();
  Gps_Data readGPS();
  Bno_Data readBNO();
  float readVbat();

private:
  /* template function to get data from computer of serial */
  template <typename T>
  void getData(T* dat){
    const int len = sizeof(*dat);
    char bytes[len];
    while(true){
      if(Serial.available() == len){
        for(int i = 0;i < len; i++){
          bytes[i]= Serial.read();
        }
        memcpy(bytes,dat,len);
      }
    }
  }

  void float2Bytes(float val, byte* bytes_array);
  float bytes2Float(byte* bytes_array);
  void request(byte select);
  byte requestMsg[5];
  elapsedMicros global_time;
};







#endif
