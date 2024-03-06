
#pragma once

#include <ButtonDebounceTask.h>

class MQTTHandlerTask;

class MainButtonTask: public ButtonDebounceTask
{
  public:
  MainButtonTask(int pin, MQTTHandlerTask& mqttHandler);

  void ButtonPressed();
  void ButtonReleased();

  private:
  MQTTHandlerTask& mqttTask;

};

