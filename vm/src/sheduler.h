/*
  ╔══════════════════════════════════════════════════════════╗
  ║                         sheduler.h
  ║ defines functions related or used by the sheduler.
  ╚══════════════════════════════════════════════════════════╝
*/

#ifndef SHEDULER_H
#define SHEDULER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "vm.h"

/* ╔═ sheduler_process ═══════════════════════════════╗
 * ║
 * ║ Executes one instruction of the given VM.
 * ║
 * ║ parameters:
 * ║ - vm: VM to execute.
 * ║
 * ║ returns:
 * ║ -  0 Instruction executed
 * ║ - <0 Instruction error/VM died
 * ║
 * ╚════════════════════════════════════════════════╝
 */
int sheduler_process( vm_t *vm );
#endif
