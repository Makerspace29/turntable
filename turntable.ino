/*
Внешнее ПО определяет количество шагов и отправляет его через последовательный порт в формате -> m1000,50
  где:
  1000 -> Количество шагов
  50   -> Скорость
При запуске вращения ардуино пишет mstart на последовательном порту, при завершении вращения пишет mend.
*/

#include <AccelStepper.h>

#define INPUT_SIZE 32

#define enablePin 2
#define dirPin 3
#define stepPin 4

// M420_1
// EN 2
// DIR 3
// PUL 4

// M420_2
// EN 5
// DIR 6
// PUL 7

// ENDSWITCH_1 11 (мал)
// ENDSWITCH_2 13 (бол)

#define motorInterfaceType 1

bool stepperStart = false;
int stepperPosition = 0;
int stepperSpeed = 0;
int stepperMaxSpeed = 0;
char inputBuf[INPUT_SIZE + 1];

AccelStepper myMotor(motorInterfaceType, stepPin, dirPin);  

void setup()
{
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, HIGH);

    Serial.begin(9600);
    Serial.setTimeout(1000);
    Serial.println("ts12");

    myMotor.setAcceleration(500);
}

void loop()
{
  isSerialAvailable();
  startRevolution(); 
}

bool isSerialAvailable()
{ 
    if( !Serial.available() )
    {
        return false;
    }
    byte size = Serial.readBytesUntil('.', inputBuf, INPUT_SIZE);
    inputBuf[size] = 0;
    if( inputBuf[0] != 'm' )
    {
        Serial.println("ts12");
        return false;
    }
    char* separator = strchr(inputBuf+1, ',');
    if (separator != 0)
    {
        *separator = 0;
        stepperPosition = atoi(inputBuf+1);
        ++separator;
        stepperSpeed = atoi(separator);
    }
    if( stepperPosition != 0 && stepperSpeed != 0 )
    {
        stepperStart = true;
        return true;
    }
    else
    {
        stepperStart = false;
        stepperPosition = 0;
        stepperSpeed = 0;
        return false;
    }
}

void startRevolution(){
   if (stepperStart)
    {
        Serial.println("mstart");
        Serial.flush();
        myMotor.setMaxSpeed(stepperSpeed);
        myMotor.moveTo(stepperPosition);
        myMotor.runToPosition();
        myMotor.setCurrentPosition(0);
        Serial.println("mend");
        Serial.flush();
        stepperStart = false;
        stepperPosition = 0;
        stepperSpeed = 0;   
    }
  }
