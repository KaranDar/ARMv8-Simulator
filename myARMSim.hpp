//
//  myARMSim.hpp
//  ARMv8_Sim
//
//  Created by Karan Daryani on 27/10/18.
//  Copyright © 2018 Karan Daryani. All rights reserved.
//

#ifndef myARMSim_hpp
#define myARMSim_hpp

#include <stdio.h>

void init_proc(); // function to initialize the code array and memory array

void fetch(); // function to get all the instruction set from the file

void execute(); // function to execute all the instructions

void MOV(int value);

void ADD(int value);

void ADDI(int value);

void SUB(int value);

void SUBI(int value);

void MUL(int value);

void DIV(int value);

void AND(int value);

void ANDI(int value);

void OR(int value);

void LSL(int value);

void LSR(int value);

void STUR(int value);

void LDUR(int value);

int JZ(int value);

int JNZ(int value);

int JMP(int value);

int BR(int value);

int CBZ(int value);

int CBNZ(int value);

int BL(int value);

void dump_reg();

void dump_CODE();

void dump_flag();

void print_stack();

#endif /* myARMSim_hpp */
