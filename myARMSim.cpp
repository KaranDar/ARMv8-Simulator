//
//  myARMSim.cpp
//  ARMv8_Sim
//
//  Created by Karan Daryani on 27/10/18.
//  Copyright © 2018 Karan Daryani. All rights reserved.
//
#define array_max 4444444

#include "myARMSim.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

static uint16_t lengthcode=0;

//Register file
static signed long long X[32];

//flags
static bool N=0,C=0,V=0,Z=0;

//TEMPCODE
static string TEMPCODE[4000];

//Starting Address of Code
static long long startaddrcode=0;

//Starting Address of Data
static long long startaddrdata=0;

static long long stackpointer=0,stackpointerinit=0;

static int DATA[array_max];

int flag[32];

void addFlag(long long c, long long a, long long b){
    N=0;Z=0;V=0;C=0;
    
    N=c>>31;
    if(c==0){Z=1;}
    
    if((a>>63)==0 && (b>>63)==0 && (c>>63)==1 ){V=1;}
    if((a>>63)==1 && (b>>63)==1 && (c>>63)==0 ){V=1;C=1;}
    
    if((a>>63)==1 && (b>>63)==0 && (c>>63)==0 ){C=1;}
    if((a>>63)==0 && (b>>63)==1 && (c>>63)==0 ){C=1;}
    
}

void subFlag(long long c, long long a, long long b){
    N=0;Z=0;V=0;C=0;
    
    N=c>>63;
    if(c == 0){Z=1;}
    
    if((a>>63)==0 && (b>>63)==1 && (c>>63==1)){C=1;}
    if((a>>63)==1 && (b>>63)==0 && (c>>63==0)){V=1;}
    if((a>>63)==0 && (b>>63)==0 && (c>>63==1)){C=1;}
}

void init_proc(){
    for(int i = 0 ; i < 32 ; i++)
    {
        X[i] = 0;
        //printf("Register %.2d = %d \n", i,X[i]);
    }
    N = C = V = Z = 0;
    int dataptr=0, codeptr=0;
    // to get .code
    for (codeptr=0;codeptr<lengthcode;codeptr++){
        string temp;
        temp = TEMPCODE[codeptr];
        if (temp == ".code")
        {
            string temp = TEMPCODE[codeptr+1];
            string toErase1 = "[";
            string toErase2 = "]";
            size_t pos = std::string::npos;
            while ((pos = temp.find(toErase1))!= std::string::npos)
            {
                temp.erase(pos, toErase1.length());
            }
            while ((pos = temp.find(toErase2))!= std::string::npos)
            {
                temp.erase(pos, toErase2.length());
            }
            startaddrcode = stoll(temp, NULL, 16);
            
            printf("Start Address of Code is : %llX \n",startaddrcode);
//            long long startadd=startaddrcode;
//            printf("Start Address of Code is : %llX \n",startadd);
//            string CODE[startadd];
            //TODO: Ask Professor about how to
        }
    }
    //to get .data
    for(dataptr=0;dataptr<lengthcode;dataptr++){
        string temp;
        temp = TEMPCODE[dataptr];
        if (temp == ".data")
        {
            string temp = TEMPCODE[dataptr+1];
            string toErase1 = "[";
            string toErase2 = "]";
            size_t pos = std::string::npos;
            while ((pos = temp.find(toErase1))!= std::string::npos)
            {
                temp.erase(pos, toErase1.length());
            }
            while ((pos = temp.find(toErase2))!= std::string::npos)
            {
                temp.erase(pos, toErase2.length());
            }
            startaddrdata = stoll(temp, NULL);
            stackpointer = startaddrdata;
            stackpointerinit = startaddrdata;
            printf("Start Address of Data is : %llX \n",stackpointer);
            printf("Initial Address of Data is : %llX \n",stackpointerinit);
        }
    }
}

void fetch(){
    ifstream fin;
    fin.open("AssemblyCode.txt");
    
    string oneline,a,b,c,d;
    while (!fin.eof()){
        getline(fin, oneline);
        stringstream(oneline)>>a>>b>>c>>d;
        TEMPCODE[lengthcode] = a;
        lengthcode++;
        TEMPCODE[lengthcode] = b;
        lengthcode++;
        TEMPCODE[lengthcode] = c;
        lengthcode++;
        TEMPCODE[lengthcode] = d;
        lengthcode++;
        a="";b="";c="";d="";
    }
    fin.close();
    //NoOfLines=lengthcode/4;
    for(int x=0;x<4000;x++)
    {
            string temp = TEMPCODE[x];
            string toErase1 = ",";
            string toErase2 = "#";
            string toErase3 = "[";
            string toErase4 = "]";
        
            size_t pos = std::string::npos;
            while ((pos = temp.find(toErase1))!= std::string::npos)
            {
                temp.erase(pos, toErase1.length());
            }
            while ((pos = temp.find(toErase2))!= std::string::npos)
            {
                temp.erase(pos, toErase2.length());
            }
            while ((pos = temp.find(toErase3))!= std::string::npos)
            {
                temp.erase(pos, toErase3.length());
            }
            while ((pos = temp.find(toErase4))!= std::string::npos)
            {
                temp.erase(pos, toErase4.length());
            }
            //cout<<temp<<endl;
            TEMPCODE[x] = temp;
    }
    for(int x=0;x<lengthcode;x+=4)
    {
        printf("%s %s %s %s \n",TEMPCODE[x].c_str(),TEMPCODE[x+1].c_str(),TEMPCODE[x+2].c_str(),TEMPCODE[x+3].c_str());
    }
}

void execute(){
    
    printf("In execution \n");
    for (int i=0;i<lengthcode;i++){
        string temp;
        temp = TEMPCODE[i];
        if (temp == "ADD")
        {
            ADD(i);
            //printf("ADD\n");
        }
        else if (temp == "ADDI")
        {
            ADDI(i);
            //printf("ADDI\n");
        }
        else if (temp == "SUB")
        {
            SUB(i);
            //printf("SUB\n");
        }
        else if (temp == "SUBI")
        {
            SUBI(i);
            //printf("SUBI\n");
        }
        else if (temp == "MOV")
        {
            MOV(i);
            //printf("MOV\n");
        }
        else if (temp == "MUL")
        {
            MUL(i);
            //printf("MUL\n");
        }
        else if (temp == "DIV")
        {
            DIV(i);
            //printf("DIV\n");
        }
        else if (temp == "AND")
        {
            AND(i);
            //printf("AND\n");
        }
        else if (temp == "ANDI")
        {
            ANDI(i);
            //printf("ANDI\n");
        }
        else if (temp == "OR")
        {
            OR(i);
            //printf("OR\n");
        }
        else if (temp == "LSL")
        {
            LSL(i);
            //printf("LSL\n");
        }
        else if (temp == "LSR")
        {
            LSR(i);
            //printf("LSR\n");
        }
        else if (temp == "LDUR")
        {
            LDUR(i);
            //printf("LDUR\n");
        }
        else if (temp == "STUR")
        {
            STUR(i);
            //printf("STUR\n");
        }
        else if (temp == "JZ")
        {
            int tempi;
            tempi=JZ(i);
            i=tempi;
            //printf("JZ\n");
        }
        else if (temp == "JNZ")
        {
            int tempi;
            tempi=JNZ(i);
            i=tempi;
            //printf("JNZ\n");
        }
        else if (temp == "JMP")
        {
            int tempi;
            tempi=JMP (i);
            i=tempi;
            //printf("JMP\n");
        }
        else if (temp == "CBZ")
        {
            int tempi;
            tempi = CBZ(i);
            i=tempi;
            //printf("CBZ\n");
        }
        else if (temp == "CBNZ")
        {
            int tempi;
            tempi = CBNZ(i);
            i=tempi;
            //printf("CBNZ\n");
        }
        else if (temp == "BR")
        {
            //int tempi;
            //tempi =  BR(i);
            //i=tempi;
            //printf(" BR %d \n",i);
            //printf("BR\n");
        }
        else if (temp == "BL")
        {
            //int tempi;
            //tempi =  BL(i);
            //i=tempi;
            //printf("BR\n");
        }
        else if ( temp == "hlt")
        {
            dump_reg();
            dump_flag();
            printf("Stack Pointer = %d \n",stackpointer);
            print_stack();
            exit(5);
        }
        
    }
}

void MOV(int value){
    string reg = TEMPCODE[value+1];
    string toErase = "X";
    size_t pos = std::string::npos;
    while ((pos = reg.find(toErase))!= std::string::npos)
    {
        reg.erase(pos, toErase.length());
    }
    int regno = stoi(reg);
    string tem = TEMPCODE[value+2];
    long long temp = stoll(tem, NULL, 16);
    X[regno] = temp;
    //printf("X[%d] = %d \n",regno,X[regno]);
}

void ADD(int value){
    string destreg = TEMPCODE[value+1];
    string operand1 = TEMPCODE[value+2];
    string operand2 = TEMPCODE[value+3];
    string toErase = "X";
    size_t pos = std::string::npos;
    while ((pos = destreg.find(toErase))!= std::string::npos)
    {
        destreg.erase(pos, toErase.length());
    }
    while ((pos = operand1.find(toErase))!= std::string::npos)
    {
        operand1.erase(pos, toErase.length());
    }
    while ((pos = operand2.find(toErase))!= std::string::npos)
    {
        operand2.erase(pos, toErase.length());
    }
    if(destreg=="SP"){
        if(operand1=="SP")
        {
            int operand2no = stoi(operand2);
            stackpointer = stackpointer + X[operand2no];
        }
        else{
            int operand1no = stoi(operand1);
            int operand2no = stoi(operand2);
            stackpointer = X[operand1no] + X[operand2no];
        }
    }
    else{
        if(operand1=="SP")
        {
            int destregno = stoi(destreg);
            int operand2no = stoi(operand2);
            X[destregno] = stackpointer + X[operand2no];
            addFlag(X[destregno], stackpointer, X[operand2no]);
        }
        else{
        int destregno = stoi(destreg);
        int operand1no = stoi(operand1);
        int operand2no = stoi(operand2);
        signed long long temp = X[operand1no] + X[operand2no];
        X[destregno] = temp;
        addFlag(X[destregno], X[operand1no], X[operand2no]);
        }
    }

}

void SUB(int value){
    string destreg = TEMPCODE[value+1];
    string operand1 = TEMPCODE[value+2];
    string operand2 = TEMPCODE[value+3];
    string toErase = "X";
    size_t pos = std::string::npos;
    while ((pos = destreg.find(toErase))!= std::string::npos)
    {
        destreg.erase(pos, toErase.length());
    }
    while ((pos = operand1.find(toErase))!= std::string::npos)
    {
        operand1.erase(pos, toErase.length());
    }
    while ((pos = operand2.find(toErase))!= std::string::npos)
    {
        operand2.erase(pos, toErase.length());
    }
    if(destreg=="SP"){
        if(operand1=="SP")
        {
            int operand2no = stoi(operand2);
            stackpointer = stackpointer - X[operand2no];
        }
        else{
            int operand1no = stoi(operand1);
            int operand2no = stoi(operand2);
            stackpointer = X[operand1no] - X[operand2no];
        }
    }
    else{
        int destregno = stoi(destreg);
        int operand1no = stoi(operand1);
        int operand2no = stoi(operand2);
        signed long long temp = X[operand1no] - X[operand2no];
        X[destregno] = temp;
        subFlag(X[destregno], X[operand1no], X[operand2no]);
    }
}

void ADDI(int value){
    string destreg = TEMPCODE[value+1];
    string operand1 = TEMPCODE[value+2];
    string operand2 = TEMPCODE[value+3];
    string toErase = "X";
    if(destreg=="SP"){
        if(operand1=="SP")
        {
            stackpointer=stackpointer+stoi(operand2);
        }
        else{
            size_t pos = std::string::npos;
            while ((pos = destreg.find(toErase))!= std::string::npos)
            {
                destreg.erase(pos, toErase.length());
            }
            while ((pos = operand1.find(toErase))!= std::string::npos)
            {
                operand1.erase(pos, toErase.length());
            }
            //int destregno = stoi(destreg);
            int operand1no = stoi(operand1);
            int operand2no = stoi(operand2);
            signed long long temp = X[operand1no] + operand2no;
            stackpointer = temp;
            //flag_update(destregno);
            //printf("X[%d] = %d \n",destregno,X[destregno]);
        }
    }
    else{
        size_t pos = std::string::npos;
        while ((pos = destreg.find(toErase))!= std::string::npos)
        {
            destreg.erase(pos, toErase.length());
        }
        while ((pos = operand1.find(toErase))!= std::string::npos)
        {
            operand1.erase(pos, toErase.length());
        }
        int destregno = stoi(destreg);
        int operand1no = stoi(operand1);
        int operand2no = stoi(operand2);
        signed long long temp = X[operand1no] + operand2no;
        X[destregno] = temp;
        addFlag(X[destregno], X[operand1no], operand2no);
        
        //printf("X[%d] = %d \n",destregno,X[destregno]);
    }
}



void SUBI(int value){
    string destreg = TEMPCODE[value+1];
    string operand1 = TEMPCODE[value+2];
    string operand2 = TEMPCODE[value+3];
    string toErase = "X";
    if(destreg=="SP"){
        if(operand1=="SP")
        {
            stackpointer=stackpointer-stoi(operand2);
        }
        else{
            size_t pos = std::string::npos;
            while ((pos = destreg.find(toErase))!= std::string::npos)
            {
                destreg.erase(pos, toErase.length());
            }
            while ((pos = operand1.find(toErase))!= std::string::npos)
            {
                operand1.erase(pos, toErase.length());
            }
            //int destregno = stoi(destreg);
            int operand1no = stoi(operand1);
            int operand2no = stoi(operand2);
            signed long long temp = X[operand1no] - operand2no;
            stackpointer = temp;
            //flag_update(destregno);
            //printf("X[%d] = %d \n",destregno,X[destregno]);
        }
    }
    else{
        size_t pos = std::string::npos;
        while ((pos = destreg.find(toErase))!= std::string::npos)
        {
            destreg.erase(pos, toErase.length());
        }
        while ((pos = operand1.find(toErase))!= std::string::npos)
        {
            operand1.erase(pos, toErase.length());
        }
        int destregno = stoi(destreg);
        int operand1no = stoi(operand1);
        int operand2no = stoi(operand2);
        signed long long temp = X[operand1no] - operand2no;
        X[destregno] = temp;
        //flag_update(destregno);
        subFlag(X[destregno], X[operand1no], operand2no);
        //printf("X[%d] = %d \n",destregno,X[destregno]);
    }
}

void DIV(int value){
    string destreg = TEMPCODE[value+1];
    string operand1 = TEMPCODE[value+2];
    string operand2 = TEMPCODE[value+3];
    string toErase = "X";
    size_t pos = std::string::npos;
    while ((pos = destreg.find(toErase))!= std::string::npos)
    {
        destreg.erase(pos, toErase.length());
    }
    while ((pos = operand1.find(toErase))!= std::string::npos)
    {
        operand1.erase(pos, toErase.length());
    }
    while ((pos = operand2.find(toErase))!= std::string::npos)
    {
        operand2.erase(pos, toErase.length());
    }
    int destregno = stoi(destreg);
    int operand1no = stoi(operand1);
    int operand2no = stoi(operand2);
    long long temp = X[operand1no] / X[operand2no];
    X[destregno] = temp;
    //printf("X[%d] = %d \n",destregno,X[destregno]);
}

void MUL(int value){
    string destreg = TEMPCODE[value+1];
    string operand1 = TEMPCODE[value+2];
    string operand2 = TEMPCODE[value+3];
    string toErase = "X";
    size_t pos = std::string::npos;
    while ((pos = destreg.find(toErase))!= std::string::npos)
    {
        destreg.erase(pos, toErase.length());
    }
    while ((pos = operand1.find(toErase))!= std::string::npos)
    {
        operand1.erase(pos, toErase.length());
    }
    while ((pos = operand2.find(toErase))!= std::string::npos)
    {
        operand2.erase(pos, toErase.length());
    }
    int destregno = stoi(destreg);
    int operand1no = stoi(operand1);
    int operand2no = stoi(operand2);
    long long temp = X[operand1no] * X[operand2no];
    X[destregno] = temp;
    //printf("X[%d] = %d \n",destregno,X[destregno]);
}

void AND(int value){
    string destreg = TEMPCODE[value+1];
    string operand1 = TEMPCODE[value+2];
    string operand2 = TEMPCODE[value+3];
    string toErase = "X";
    size_t pos = std::string::npos;
    while ((pos = destreg.find(toErase))!= std::string::npos)
    {
        destreg.erase(pos, toErase.length());
    }
    while ((pos = operand1.find(toErase))!= std::string::npos)
    {
        operand1.erase(pos, toErase.length());
    }
    while ((pos = operand2.find(toErase))!= std::string::npos)
    {
        operand2.erase(pos, toErase.length());
    }
    int destregno = stoi(destreg);
    int operand1no = stoi(operand1);
    int operand2no = stoi(operand2);
    long long temp = X[operand1no] & X[operand2no];
    X[destregno] = temp;
    
}

void ANDI(int value){
    string destreg = TEMPCODE[value+1];
    string operand1 = TEMPCODE[value+2];
    string operand2 = TEMPCODE[value+3];
    string toErase = "X";
    size_t pos = std::string::npos;
    while ((pos = destreg.find(toErase))!= std::string::npos)
    {
        destreg.erase(pos, toErase.length());
    }
    while ((pos = operand1.find(toErase))!= std::string::npos)
    {
        operand1.erase(pos, toErase.length());
    }
    int destregno = stoi(destreg);
    int operand1no = stoi(operand1);
    int operand2no = stoi(operand2);
    long long temp = (X[operand1no] & operand2no);
    X[destregno] = temp;
    //printf("X[%d] = %d \n",destregno,X[destregno]);
}

void OR(int value){
    string destreg = TEMPCODE[value+1];
    string operand1 = TEMPCODE[value+2];
    string operand2 = TEMPCODE[value+3];
    string toErase = "X";
    size_t pos = std::string::npos;
    while ((pos = destreg.find(toErase))!= std::string::npos)
    {
        destreg.erase(pos, toErase.length());
    }
    while ((pos = operand1.find(toErase))!= std::string::npos)
    {
        operand1.erase(pos, toErase.length());
    }
    while ((pos = operand2.find(toErase))!= std::string::npos)
    {
        operand2.erase(pos, toErase.length());
    }
    int destregno = stoi(destreg);
    int operand1no = stoi(operand1);
    int operand2no = stoi(operand2);
    long long temp = X[operand1no]|X[operand2no];
    X[destregno] = temp;
}

void LSL(int value){
    string destreg = TEMPCODE[value+1];
    string operand1 = TEMPCODE[value+2];
    string operand2 = TEMPCODE[value+3];
    string toErase = "X";
    size_t pos = std::string::npos;
    while ((pos = destreg.find(toErase))!= std::string::npos)
    {
        destreg.erase(pos, toErase.length());
    }
    while ((pos = operand1.find(toErase))!= std::string::npos)
    {
        operand1.erase(pos, toErase.length());
    }
    int destregno = stoi(destreg);
    int operand1no = stoi(operand1);
    int operand2lsl = stoi(operand2);
    long long temp = (X[operand1no] << operand2lsl);
    X[destregno] = temp;
}
void LSR(int value){
    string destreg = TEMPCODE[value+1];
    string operand1 = TEMPCODE[value+2];
    string operand2 = TEMPCODE[value+3];
    string toErase = "X";
    size_t pos = std::string::npos;
    while ((pos = destreg.find(toErase))!= std::string::npos)
    {
        destreg.erase(pos, toErase.length());
    }
    while ((pos = operand1.find(toErase))!= std::string::npos)
    {
        operand1.erase(pos, toErase.length());
    }
    int destregno = stoi(destreg);
    int operand1no = stoi(operand1);
    int operand2lsr = stoi(operand2);
    long long temp = (X[operand1no] >> operand2lsr);
    X[destregno] = temp;
}

int JZ(int value){
    int ptr1=0,ptr2=0;
    string Label = TEMPCODE[value +1];
    Label = Label + ":";
    for (ptr1=0;ptr1<lengthcode;ptr1++){
        string temp;
        temp = TEMPCODE[ptr1];
        if (temp == Label){
            ptr2 = ptr1;
        }
    }
    if( Z == 1 ){
        return ptr2;
    } else {
        return value+1;
    }
}

int JNZ(int value){
    int ptr1=0,ptr2=0;
    string Label = TEMPCODE[value +1];
    Label = Label + ":";
    for (ptr1=0;ptr1<lengthcode;ptr1++){
        string temp;
        temp = TEMPCODE[ptr1];
        if (temp == Label){
            ptr2 = ptr1;
        }
    }
    if( Z == 0 ){
        return ptr2;
    } else {
        return value+2;
    }
}

int JMP(int value){
    int ptr1=0,ptr2=0;
    string Label = TEMPCODE[value +1];
    Label = Label + ":";
    for (ptr1=0;ptr1<lengthcode;ptr1++){
        string temp;
        temp = TEMPCODE[ptr1];
        if (temp == Label){
            ptr2 = ptr1;
        }
    }
    return ptr2;
}

int CBZ(int value){
    string regtocheck = TEMPCODE[value+1];
    string toErase = "X";
    size_t pos = std::string::npos;
    while ((pos = regtocheck.find(toErase))!= std::string::npos)
    {
        regtocheck.erase(pos, toErase.length());
    }
    int regcheck = stoi(regtocheck);
    int ptr1=0,ptr2=0;
    string Label = TEMPCODE[value+2];
    Label = Label + ":";
    for (ptr1=0;ptr1<lengthcode;ptr1++){
        string temp;
        temp = TEMPCODE[ptr1];
        if (temp == Label){
            ptr2 = ptr1;
        }
    }
    if( X[regcheck] == 0){
        return ptr2;
    } else {
        return value+3;
    }
}

int CBNZ(int value){
    string regtocheck = TEMPCODE[value+1];
    string toErase = "X";
    size_t pos = std::string::npos;
    while ((pos = regtocheck.find(toErase))!= std::string::npos)
    {
        regtocheck.erase(pos, toErase.length());
    }
    int regcheck = stoi(regtocheck);
    int ptr1=0,ptr2=0;
    string Label = TEMPCODE[value+2];
    Label = Label + ":";
    for (ptr1=0;ptr1<lengthcode;ptr1++){
        string temp;
        temp = TEMPCODE[ptr1];
        if (temp == Label){
            ptr2 = ptr1;
        }
    }
    if( X[regcheck] != 0){
        return ptr2;
    } else {
        return value+3;
    }
}

int BR(int value){
    long long returnvalue=0;
    if (TEMPCODE[value+1] == "X30"){
        if (X[30] == stackpointerinit){
            dump_reg();
            dump_flag();
            printf("Stack Pointer = %d \n",stackpointer);
            print_stack();
            exit(5);
        } else {
        returnvalue = X[30];
        }
    }
    return returnvalue;
}

int BL(int value){
    X[30]=value+2;
    int ptr1=0,ptr2=0;
    string Label = TEMPCODE[value +1];
    Label = Label + ":";
    for (ptr1=0;ptr1<lengthcode;ptr1++){
        string temp;
        temp = TEMPCODE[ptr1];
        if (temp == Label){
            ptr2 = ptr1;
        }
    }
    return ptr2;
}

void STUR(int value){
    string reg = TEMPCODE[value+1];
    string operand1 = TEMPCODE[value+2];
    string operand2 = TEMPCODE[value+3];
    string toErase = "X";
    size_t pos = std::string::npos;
    while ((pos = reg.find(toErase))!= std::string::npos)
    {
        reg.erase(pos, toErase.length());
    }
    int regno = stoi(reg);
    int operand2no = stoi(operand2);
    DATA[stackpointer+operand2no] = X[regno];
    //printf("Stack Pointer Now Is = %lld \n",stackpointer);
    
}

void LDUR(int value){
    string reg = TEMPCODE[value+1];
    string operand1 = TEMPCODE[value+2];
    string operand2 = TEMPCODE[value+3];
    string toErase = "X";
    size_t pos = std::string::npos;
    while ((pos = reg.find(toErase))!= std::string::npos)
    {
        reg.erase(pos, toErase.length());
    }
    int regno = stoi(reg);
    int operand2no = stoi(operand2);
    X[regno] = DATA[stackpointer+operand2no];
}

void dump_reg(){
    printf("|=======================|\n");
    for(int i = 0 ; i < 32 ; i++)
    {
        printf("Register %.2d = x%.16llX \n",i,X[i]);
    }
}

void print_stack(){
    for(int i = (stackpointerinit-12) ; i <= (stackpointerinit-1) ; i++)
    {
        printf("%d ",DATA[i]);
    }
    printf("\n");
}

void dump_flag(){
    printf("N=%d, Z=%d, C=%d, V=%d \n",N,Z,C,V);
}
