/*
 * Simple program for FRDM-K64F with CAN loopback 
 *
 * Implemented using a time-triggered scheduler.
 *
 * DK - 02-Feb-2019
 */


/* TODO:
 * comments like this give some additional guidance for the assignment
 * with a view to working alone rather than in a pair.
 * Reflecting the current distancing situation.
 *                                              Alun     27 March 2020
 *   I've commented out sections of code that are
 *   unneeded for the assignment.
 *   Please delete commented-out, unneeded, and redundant sections
 *   of code before submitting the source.
 *   It makes the job of marking and reviewing easier.
 */
 

#include <stdbool.h>
#include <stdint.h>
#include <mbed.h>
#include <stdio.h>
#include "can.h"
#include "scheduler.h"

static DigitalOut red(LED_RED);
static Serial pc(USBTX, USBRX, 115200);
uint32_t txCount;
uint32_t rxCount;

static void led1ToggleTask(void);

/* we don't need read and write tasks
static void canWriteTask(void);
static void canReadTask(void);
*/

/* TODO:
 * Declare task handlers for TempT and TachoT
 */
void TempT(void);
void TachoT(void);

int main()
{
    red = 0;

	/* TODO:
	 * Normally you would CAN connect two boards,
	 * If you are working alone, or with a single board
	 * then you will have to use the CAN adaptor in
	 * `loopback' mode.
	 */
    canInit(BD125000, true);

    pc.printf("EngineMonitor -- Loopback test\n");

    schInit();
    schAddTask(led1ToggleTask, 0, 500);

	/* YOU don't need the read and write tasks from the exercises.
	 * but they ARE good examples to follow
	 */
	/* comment out unneeded code:
        schAddTask(canWriteTask, 3, 250);
        schAddTask(canReadTask, 5, 100);
	*/

	/* TODO:
	 * Add tasks TempT and TachoT to the scheduler
	 * with appropriate delays and periods.
	 */

    schStart();

	/* TODO:
	 * If you are working alone with the CAN adaptor in `loopback' mode
	 * then you will need to send a single message to become the first 
	 * ReqT message for TempT.
	 */

    while (true) {
        schDispatch();
    }
}

/* TODO: TachoT task
 * The handler for TachoT task
 * should be called every 20ms from the scheduler
 */
void TachoT(void) {
	/* TODO:
	 * Declare needed variables.
	 * remember to use `static' for any
	 * variables where you want the value to persist from one call 
	 * to the next.
	 */

	/* TODO:
	 * Handle the Tacho data.
	 * As we don't have a real tachometer you can use a 
	 * constant value, a simple incrementing value, 
	 * or whatever you can invent.
	 */

	/* TODO:
	 * Remember this is an event handler so DOES NOT have
	 * a while loop.
	 * it MUST exit and pass control back to the scheduler
	 */
}

/* TODO: TempT task
 * Handler for the TempT task.
 * Note: if you are working alone the `loopback' mode
 * echoes the messge back, so each AckT becomes the next ReqT.
 */
void TempT(void)
{
	/* TODO:
	 * Declare needed variables.
	 * remember to use `static' for any
	 * variables where you want the value to persist from one call 
	 * to the next.
	 */

	/* TODO: Poll the CAN bus for arrival of ReqT messages */

	/* TODO: if message available, read and act on it */
	/* TODO: if a message was read send a ReqT message. */

	/* TODO:
	 * Handle the Temp data.
	 * As we don't have a real thermometer you can use a 
	 * constant value, a simple incrementing value, 
	 * or whatever you can invent.
	 */

	/* TODO:
	 * Remember this is an event handler so DOES NOT have
	 * a while loop.
	 * it MUST exit and pass control back to the scheduler
	 */
}


static void led1ToggleTask(void)
{
    red = 1 - red;
}

/* don't need the write task, but it does have useful code.
void canWriteTask(void)
{

    static canMessage_t txMsg = { 0x23, 8, 0, 0 };
    bool txOk;

    // Transmit message on CAN 
    txOk = canWrite(&txMsg);
    if (txOk) {
        txCount += 1;
        txMsg.dataA = txCount;
        txMsg.dataB = txCount;
    }
}
*/

/* don't need the read task, but it does have useful code
void canReadTask(void)
{

    static canMessage_t rxMsg;

    if (canReady()) {
        canRead(&rxMsg);
        pc.printf
            ("ID: 0x%lx LEN: 0x%01lx DATA_A: 0x%08lx DATA_B: 0x%08lx\n",
             rxMsg.id, rxMsg.len, rxMsg.dataA, rxMsg.dataB);
        rxCount += 1;
    }
}
*/
