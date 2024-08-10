#include "Agent.h"
#include <string.h>

Agent::Agent() {
}

Agent::~Agent() {
  stop();
}

Task Handle_t Agent::xHandle=NULL;

bool Agent::start( const char *name, UBaseType_t priority ) { 
  //assume that name is a proper length
  BaseType_t res;
  strcpy(pName,name);

  res = xTaskCreate(
      Agent::vTask,
      pName,
      getMaxStackSize(); //in words
      priority,
      &xHandle
    );
  return ( res == pdPASS );
}

void Agent::vTask ( void * pvParameters ) {
  Agent *task = (Agent*) pvParameters;
  if (task != NULL) {
    task->run();
  }
}

TaskHandle_t Agent::getTask() {
  return xHandle;
}

unsigned int Agent::getStackHighWater() {
  if (xHandle!=NULL)
    return uxTaskGetStackHighWaterMark(xHandle);
  else
    return 0;
}

void Agent::stop() {
  if(xHandle!=NULL) {
    vTaskDelete( xHandle );
    xHandle = NULL;
  }
}
      
