#include "Agent.h"
#include <string.h>

Agent::Agent() {
}

Agent::~Agent() {
  stop();
}

//stop task
void Agent::stop() {
  if(xHandle!=NULL) {
    vTaskDelete( xHandle );
    xHandle = NULL;
  }
}

//get high water mark for stack
unsigned int Agent::getStakHighWater() {
  if(xHandle!=NULL)
    return uxTaskGetStackHighWaterMark(xHandle);
  else
    return 0;
}

TaskHandle_t Agent::getTask(){
  return xHandle;
}

//start the task
bool Agent::start(const char *name, UBaseType_t priority){
	BaseType_t res;

	if (strlen(name) >= MAX_NAME_LEN){
		memcpy(pName, name, MAX_NAME_LEN);
		pName[MAX_NAME_LEN-1]=0;
	} else {
		strcpy(pName, name);
	}
	res = xTaskCreate(
			Agent::vTask,       /* Function that implements the task. */
		pName,   /* Text name for the task. */
		getMaxStackSize(),             /* Stack size in words, not bytes. */
		( void * ) this,    /* Parameter passed into the task. */
		priority,/* Priority at which the task is created. */
		&xHandle
	);
	return (res == pdPASS);
}

//internal func; freertos running the task
void Agent::vTask( void *pvParameters ){
  Agent *task = (Agent *) pvParameters;
  if(task!=NULL){
    task->run();
  }
}


