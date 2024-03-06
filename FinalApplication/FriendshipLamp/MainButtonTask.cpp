#include "MainButtonTask.h"
#include "MQTTHandler.h"


MainButtonTask::MainButtonTask(int pin, MQTTHandlerTask& mqttHandler)
:ButtonDebounceTask(pin)
, mqttTask(mqttHandler)
{
}

//These are backwards due to pullup
void MainButtonTask::ButtonPressed()
{
}

void MainButtonTask::ButtonReleased()
{
  mqttTask.publishMyColor();
}