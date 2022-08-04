/*
 * This file is part of Espruino, a JavaScript interpreter for Microcontrollers
 *
 * Copyright (C) 2013 Gordon Williams <gw@pur3.co.uk>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * ----------------------------------------------------------------------------
 * Recursive descent JIT
 * ----------------------------------------------------------------------------
 */
#ifdef ESPR_JIT
#ifndef JSJITC_H_
#define JSJITC_H_

#include "jsutils.h"
#include "jsjit.h"

#define DEBUG_JIT jsjcDebugPrintf
// Called to print debug info - best to use DEBUG_JIT so we can disable debug lines for final compiles though
void jsjcDebugPrintf(const char *fmt, ...);

typedef enum {
  JSJVT_INT,
  JSJVT_JSVAR
} JsjValueType;

typedef enum {
  JSJAC_EQ, // 0
  JSJAC_NE,
  JSJAC_CS,
  JSJAC_CC,
  JSJAC_MI,
  JSJAC_PL,
  JSJAC_VS,
  JSJAC_VC,
  JSJAC_HI,
  JSJAC_LI,
  JSJAC_GE,
  JSJAC_LT,
  JSJAC_GT,
  JSJAC_LE,
} JsjAsmCondition;

typedef enum {
  JSJAR_r0,
  JSJAR_r1,
  JSJAR_r2,
  // ...
  JSJAR_SP = 13,
  JSJAR_LR = 14,
  JSJAR_PC = 15,
} JsjAsmReg;



// Called before start of JIT output
void jsjcStart();
// Called when JIT output stops
JsVar *jsjcStop();
// Called before start of a block of code. Returns the old code jsVar that should be passed into jsjcStopBlock
JsVar *jsjcStartBlock();
// Called when JIT output stops, pass it the return value from jsjcStartBlock. Returns the code parsed in the block
JsVar *jsjcStopBlock(JsVar *oldBlock);
// Emit a whole block of code
void jsjcEmitBlock(JsVar *block);
// Get what byte we're at in our code
int jsjcGetByteCount();

// Add 16 bit literal
void jsjcLiteral16(int reg, bool hi16, uint16_t data);
// Add 32 bit literal
void jsjcLiteral32(int reg, uint32_t data);
// Add 64 bit literal in reg,reg+1
void jsjcLiteral64(int reg, uint64_t data);
// Call a function
#ifdef DEBUG_JIT_CALLS
void _jsjcCall(void *c, const char *name);
#define jsjcCall(c) _jsjcCall(c, STRINGIFY(c))
#else
void jsjcCall(void *c);
#endif
// Store a string of data and put the address in a register. Returns the length
int jsjcLiteralString(int reg, JsVar *str, bool nullTerminate);
// Compare a register with a literal. jsjcBranchConditionalRelative can then be called
void jsjcCompareImm(int reg, int literal);
// Jump a number of bytes forward or back
void jsjcBranchRelative(int bytes);
// Jump a number of bytes forward or back, based on condition flags
void jsjcBranchConditionalRelative(JsjAsmCondition cond, int bytes);
// Move one register to another
void jsjcMov(int regTo, int regFrom);
// Move negated register
void jsjcMVN(int regTo, int regFrom);
// regTo = regTo & regFrom
void jsjcAND(int regTo, int regFrom);
// Push a register onto the stack
void jsjcPush(int reg, JsjValueType type);
// Pop off the stack to a register
JsjValueType jsjcPop(int reg);
// Add a value to the stack pointer (only multiple of 4)
void jsjcAddSP(int amt);
// Subtract a value from the stack pointer (only multiple of 4)
void jsjcSubSP(int amt);
// reg = mem[regAddr + offset]
void jsjcLoadImm(int reg, int regAddr, int offset);
// mem[regAddr + offset] = reg
void jsjcStoreImm(int reg, int regAddr, int offset);

void jsjcPushAll();
void jsjcPopAllAndReturn();

#endif /* JSJITC_H_ */
#endif /* ESPR_JIT */
