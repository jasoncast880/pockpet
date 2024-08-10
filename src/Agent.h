/*
 * Agent.h
 *
 * Abstract Agent Interface
 * Classes will derive from agent and use its methods for telemetry and direct control via rtos kernel.
 * Based on jondurrant's task implementation in FreeRTOS using agent interfaces.
 * Author: J Castillanes
 */

#include "FreeRTOS.h"
#include "task.h"

#define MAX_NAME_LEN 20

class Agent {
  public:
    Agent();
    virtual ~Agent();

    virtual bool start( const char *name, UBaseType_t priority );  
    virtual void stop();
    virtual unsigned int getStackHiWater(); //approaching zero implies overflow risk
    virtual TaskHandle_t getTask(); //i'm not sure what this does exactly

  protected:
    static void vTask( void *pvParameters); //start the task via static function (siilar to how you usually do it)
    virtual void run()=0; //task main loop to be implemented by the derived class
    virtual configSTACK_DEPTH_TYPE getMaxStackSize()=0;

    //The Task Handle
    TaskHandle_t xHandle=NULL;
    char pName[MAX_NAME_LEN];
}
