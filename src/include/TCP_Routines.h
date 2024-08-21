#ifndef TCP_ROUTINES_H_
#define TCP_ROUTINES_H_

#include "FreeRTOS.h"
#include "task.h"

#include "Agent.h"

class TCP_Routines:public Agent{
public:
  TCP_Routines();
	virtual ~TCP_Routines();

protected:

	/***
	 * Run loop for the agent.
	 */
	virtual void run();


	/***
	 * Get the static depth required in words
	 * @return - words
	 */
	virtual configSTACK_DEPTH_TYPE getMaxStackSize();

	void test();

private:

	bool testConnect();
	bool testSock();

	int xTests = 0;
	int xSuccessful = 0; //for agent object to telemetry
};

#endif /* TCP_ROUTININES_H_ */
