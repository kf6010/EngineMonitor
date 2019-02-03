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

int main () {
  red = 0;
  canInit(true);
  pc.printf("EngineMonitor -- Loopback test\n");

  schInit();
  schAddTask(led1ToggleTask, 0, 500);
  schAddTask(canWriteTask, 3, 250);
  schAddTask(canReadTask, 5, 100);

  schStart();
  
  while (true) {
    schDispatch();
  }
}

static void led1ToggleTask(void) {
  red = 1 - red;
}

void canWriteTask(void) {
  
  static canMessage_t txMsg = {0x23, 8, 0, 0};
  bool txOk;
    
  // Transmit message on CAN 
  txOk = canWrite(&txMsg);
  if (txOk) {
    txCount += 1;
    txMsg.dataA = txCount;
    txMsg.dataB = txCount;
  }
}

void canReadTask(void) {

  static canMessage_t rxMsg;
  
  if (canReady()) {           
    canRead(&rxMsg);
    pc.printf("ID: 0x%lx LEN: 0x%01lx DATA_A: 0x%08lx DATA_B: 0x%08lx\n", rxMsg.id, rxMsg.len, rxMsg.dataA, rxMsg.dataB); 
    rxCount += 1;
  }
}
