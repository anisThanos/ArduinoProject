#include "IrrigationTask.h"
#include "Arduino.h"
#include "config.h"
#include "Logger.h"

IrrigationTask::IrrigationTask(SharedState *pSharedState)
{
  this->pSharedState = pSharedState;
}

void IrrigationTask::init(int period)
{
  Task::init(period);
  pMotor = new ServoMotorImpl(SERVO_PIN);
  this->pos = 0;
  this->delta = 1;
}

void IrrigationTask::tick()
{
  pMotor->on();
  for (int i = 0; i < pSharedState->getFlow(); i++)
  {
    pMotor->setPosition(pos);
    //delay(5);
    pos += delta;
  }
  pMotor->off();

}
