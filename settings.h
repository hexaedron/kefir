//#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE
  #define DEBUG(x) Serial.println(x)
#else
  #define DEBUG(x)
#endif

// Параметры регулятора
#define kP 10.0f
#define kI 0.1f
#define kD 3.0f
#define dT 1000

// Пины
#define BTN_UP_PIN   3
#define BTN_DOWN_PIN 4
#define SENSOR_PIN  A3

#define TARGET_TEMP 38.0f

// Положение куба и строчек
#define CUBE_X     108
#define CUBE_Y       0
#define TEMP_OFFSET -5