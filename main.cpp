//
//  main.cpp
//  ARMv8_Sim
//
//  Created by Karan Daryani on 27/10/18.
//  Copyright © 2018 Karan Daryani. All rights reserved.
//

#include <iostream>
#include "myARMSim.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
    //reset_proc();
    fetch();
    printf("Fetch Done \n");
    init_proc();
    printf("Init Proc Done \n");
    execute();
    printf("Execution Done \n");
    return 1;
}
