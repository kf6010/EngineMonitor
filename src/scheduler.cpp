#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <mbed.h>
#include <scheduler.h>

/* Rewrite of the time-triggered event scheduler using the MBED-OS api.
 * We've been running into issues using the older mbed api and the use
 * of SysTick_Handler by the os
 *
 * https://os.mbed.com/docs/mbed-os/v5.15/apis/eventqueue.html
 * https://os.mbed.com/docs/mbed-os/v5.15/apis/event.html
 *
 * Dr Alun Moon
 * 13 Feb 2020
 */

EventQueue schedule;

void schInit(void) {   // initialise the scheduler
  
}

void schAddTask(               // add a task to the task set
  pVoidFunc_t task,              // the task to add
  uint32_t delay,                // the delay in ms
  uint32_t period) {             // the period
  Event<void(void)> instance = schedule.event(task);
  instance.delay(delay);
  instance.period(period);
  instance.post();
  
}

void schStart(void) {           // start ticking
}

void schDispatch(void) {          // update after a tick -- ISR
	schedule.dispatch_forever();
}

void schSleep(void) {         // go to sleep, if possible, to save power
}

