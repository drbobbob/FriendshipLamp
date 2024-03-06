#include "MainButtonTask.h"
#include "MQTTHandler.h"


MainButtonTask::MainButtonTask(int pin, MQTTHandlerTask& mqttHandler)
:ButtonDebounceTask(pin)
, mqttTask(mqttHandler)
{
}

void MainButtonTask::ButtonPressed()
{
  mqttTask.publishMyColor();
}

void MainButtonTask::ButtonReleased()
{

}