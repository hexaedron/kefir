#include "Timer3Pin2PWM.h"
#include <GyverPID.h>
#include <EncButton.h>

#define USE_MICRO_WIRE
#include <GyverOLED.h>

#define DS_CRC_USE_TABLE true
#include <microDS18B20.h>

GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
#include "oled_tools.h"
#include "simpleTimer.h"

MicroDS18B20<A3> sensor;
GyverPID regulator(10, 5, 1, 1000);
EncButton<EB_TICK, 3> btnUp;
EncButton<EB_TICK, 4> btnDown;

void setup() 
{
  initOLED();
  Wire.setClock(800000L);
  initTimer3Pin2PWM_1hz(0);
  sensor.setResolution(10);
  sensor.requestTemp();
  regulator.setLimits(0, 100);
  delay(200);
}

void loop() 
{
  Timer8  cubeTimer(100);
  Timer16 tempTimer(1000);
  float   targetTemp = 36.0;
  float   currentTemp = sensor.getTemp();
  regulator.setpoint = targetTemp;

  for(;;)
  {
    btnUp.tick();
    btnDown.tick();

    regulator.input = currentTemp;

    if(btnUp.click())
    {
      targetTemp += 0.5;
      if(targetTemp > 50)
      {
        targetTemp = 50;
      }
    }

    if(btnUp.click())
    {
      targetTemp -= 0.5;
      if(targetTemp < 25)
      {
        targetTemp = 25;
      }
    }
    
    if(cubeTimer.ready())
    {
      tickCube(108, 0);
    }

    if(tempTimer.ready())
    {
      currentTemp = sensor.getTemp();
      printTemp(currentTemp, targetTemp, -5);
      setTimer3Pin2PWMDuty(regulator.getResultTimer());
      sensor.requestTemp();
    }
  }
}
