/*
 * Simple program for FRDM-K64F with CAN loopback 
 *
 * Implemented using a time-triggered scheduler.
 *
 * DK - 02-Feb-2019
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
static void canWriteTask(void);
static void canReadTask(void);

int main()
{
    red = 0;
    canInit(BD125000, false);   /* Not in loopback mode */
    pc.printf("EngineMonitor -- \n");

    schInit();
    schAddTask(led1ToggleTask, 0, 500);
    schAddTask(canWriteTask, 3, 250);
    schAddTask(canReadTask, 5, 100);

    schStart();

    while (true) {
        schDispatch();
    }
}

static void led1ToggleTask(void)
{
    red = 1 - red;
}

/* Flag to say message has been received.
 * I can't use a semaphore here as the scheduler
 * treats these functions as one-shot actions
 * very similar to interrupt handlers
 * I CAN'T wait in a scheduled task.
 */
bool received;


void canWriteTask(void)
{

    static canMessage_t txMsg = { 0x24, 8, 0, 0 };
    bool txOk;

    if (received) {
        // Transmit message on CAN 
        txOk = canWrite(&txMsg);
        if (txOk) {
            txCount += 1;
            txMsg.dataA = txCount;
            txMsg.dataB = rxCount;
        }
        received = false;
    }
}

void canReadTask(void)
{

    static canMessage_t rxMsg;

    if (canReady()) {
        rxCount += 1;
        canRead(&rxMsg);
        pc.printf
            ("ID: %#lx LEN: %lx DATA_A: %#06lx DATA_B: %#06lx\n", rxMsg.id,
             rxMsg.len, rxMsg.dataA, rxMsg.dataB);
        rxCount += 1;
        received = true;
    }
}
