#include "Timer3Pin2PWM.h"

#define PID_OPTIMIZED_I
#include <GyverPID.h>

#include <EncButton.h>

#include "settings.h"

#define USE_MICRO_WIRE
#include <GyverOLED.h>

#define DS_CRC_USE_TABLE true
#include <microDS18B20.h>

GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
#include "oled_tools.h"
#include "simpleTimer.h"

#include <WDT.h>

MicroDS18B20<SENSOR_PIN>          sensor;
GyverPID                          regulator(kP, kI, kD, dT);
Button btnUp(BTN_UP_PIN);
Button btnDown(BTN_DOWN_PIN);

void setup() 
{
  #ifdef DEBUG_ENABLE
    Serial.begin(9600);
  #endif
  
  pinMode(BTN_UP_PIN, INPUT_PULLUP);
  pinMode(BTN_DOWN_PIN, INPUT_PULLUP);
  btnUp.setStepTimeout(500); 
  btnDown.setStepTimeout(500); 

  initOLED();
  Wire.setClock(800000L);
  initTimer3Pin2PWM_1hz(0);
  sensor.setResolution(10);
  sensor.requestTemp();
  regulator.setLimits(0, 100);
  delay(200);

  // Watchdog
  wdt_enable(WTO_4S);
}

void loop() 
{
  Timer8  cubeTimer(100);
  Timer16 tempTimer(1000);
  float   targetTemp = TARGET_TEMP;
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
      regulator.setpoint = targetTemp;
    }

    if(btnDown.click())
    {
      targetTemp -= 0.5;
      if(targetTemp < 25)
      {
        targetTemp = 25;
      }
      regulator.setpoint = targetTemp;
    }
    
    if(cubeTimer.ready())
    {
      tickCube(CUBE_X, CUBE_Y);
    }

    if(tempTimer.ready())
    {
      // Сбросим Watchdog
      wdt_reset();
      
      currentTemp = sensor.getTemp();
      DEBUG(currentTemp);
      printTemp(currentTemp, targetTemp, TEMP_OFFSET);
      setTimer3Pin2PWMDuty(regulator.getResultTimer());
      sensor.requestTemp();
    }
  }
}
