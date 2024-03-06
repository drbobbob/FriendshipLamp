
#include "Task.h"

class MQTTHandlerTask : public TimedTask
{
public:
  MQTTHandlerTask();
  void SetupMQTTHandling();

  virtual void run(uint32_t now);

  void publishMyColor();

private:
  void reconnectAttempt();
};

