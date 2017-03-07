//
//  main.cpp
//  Project Beta
//
//  Created by Wade Wilson on 2/16/17.
//  Copyright © 2017 Wade Wilson. All rights reserved.
//

#include <iostream>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <fstream>

#define LYRAND (double)rand()/RAND_MAX

using namespace::std;

class qlearner{
public:
    vector<vector<double>> Q;
};

class grid{
public:
    int xmin=0;
    int xmax;
    int ymin=0;
    int ymax;
    int goalx;
    int goaly;
    int reward;
};

class agent{
public:
    int xpos = 0;
    int ypos = 0;
    int ix = 0;
    int iy = 0;
    int pstate;
    int pinput;
    double avgr;
    double pavgr;
    int input;
    int state;
    bool win = false;
    
    double areward();
    int choice(qlearner* pqt);
    void move(grid);
    void eval(grid);
    void check(grid);
    void sense(grid);
    void update(qlearner *pqt, grid);
};

class body{
public:
    int xpos;
    int ypos;
    
};



double agent::areward(){
    int reward = -1;
    if(win){
        reward =100;
    }
    return reward;
}


void agent::move(grid world){ // will accept movement commands
    pinput=input;
    if(input == 0){
        if(ypos >= world.ymax){
            ypos = world.ymax;
        }
        else{
            ypos++;
        }
    }
    if( input == 1){
        if(ypos <= world.ymin){
            ypos = world.ymin;;
        }
        else{
            ypos -- ;
        }
    }
    if( input == 2){
        if(xpos <= world.xmin){
            xpos = world.xmin;
        }
        else{
            xpos -- ;
        }
    }
    if( input == 3){
        if(xpos >= world.xmax){
            xpos = world.xmax;
        }
        else{
            xpos ++ ;
        }
    }
    else{
        if(input<0 | input >3){
        cout << "something went wrong?"<<endl;
        }
    }
}

void agent::check(grid world){
    if(xpos >= world.xmax){
        xpos = world.xmax;
    }
    if(xpos <= world.xmin){
        xpos = world.xmin;
    }
    if(ypos <= world.ymin){
        ypos = world.ymin;
    }
    if(ypos >= world.ymax){
        ypos = world.ymax;
    }
}

void agent::eval(grid world){
    bool xwin =false;
    bool ywin = false;
    if(xpos == world.goalx){
        xwin = true;
    }
    if (ypos == world.goaly){
        ywin = true;
    }
    if(xwin && ywin){
        win = true;
    }
}

void qsetup(qlearner* pqt, int state){
}
void agent::sense(grid world){
    pstate=state;
    state = ypos*world.xmax+xpos;
    //cout<<state<<"\t"<<pstate<<endl;
}

int agent::choice(qlearner* pqt){  // Greedy choice with small exploration
    double temp = -10;
    double epsilon = .05;
    int sto = rand()%4;
    double a = LYRAND;
    if( a > epsilon){
        for(int z=0; z<4; z++){
            if(pqt->Q.at(state).at(z) > temp){
                temp = pqt->Q.at(state).at(z);
                sto=z;
            }
            
        }
    }
    //cout << sto << endl;
    return sto;
}
void reset(agent robot){
    robot.xpos=0;
    robot.ypos=0;
    robot.win=false;
}
void agent::update(qlearner* pqt, grid world){ //updates action values
    double alpha =.1;
    double gamma = .9;
    int reward = -1;
    if(win){
        reward=100;
    }
    int qmax = -10;
    for (int i = 0; i < 4; i++){
        if (pqt->Q.at(state).at(i) > qmax){
            qmax = pqt->Q.at(state).at(i);
        }
    }
    pqt->Q.at(pstate).at(pinput)=pqt->Q.at(pstate).at(pinput)+alpha*(reward+gamma*qmax - pqt->Q.at(pstate).at(pinput));
   // cout << reward << endl;
}

void testD(){
    
}

int main(){
    cout << "main start" << endl;
    srand(time(NULL));
    int episodes=1000;
    int x=10;
    int y=10;
    int num_actions=4;
    int num_state=x*y;
    agent robot;
    grid world;
    qlearner qt;
    qlearner* pqt;
    pqt = &qt;
    world.xmax=x-1;
    world.ymax=y-1;
    world.goalx=4;
    world.goaly=8;
    int stat_runs=30;
    
// setup Q table

    vector<vector<double>> tq;
    vector<double> vrow;
    for(int i=0; i<num_state; i++){
        for( int j=0; j<num_actions; j++){
            vrow.push_back(-50);
        }
        tq.push_back(vrow);
    }
    pqt->Q=tq;
    for(int i=0; i<num_state; i++){
        for( int j=0; j<num_actions; j++){
            pqt->Q.at(i).at(j)=LYRAND;
        }
    }
    ofstream outputFile;
    outputFile.open("vout.txt");
    int counting;
    for(int s=0; s<stat_runs;s++){
    for(int c=0; c<episodes; c++){
        robot.xpos=1;
        robot.ypos=1;
        robot.win=false;
        int counting = 0;
        bool notwinner=true;
        while(notwinner != robot.win){
            robot.check(world);
            robot.eval(world);
            robot.sense(world);
            robot.input = robot.choice(pqt);
            robot.move(world);
            robot.eval(world);
            robot.sense(world);
            robot.update(pqt, world);
            robot.check(world);
            robot.eval(world);
            counting++;
    }

        
        if(outputFile.is_open()){
            outputFile << counting << "\t";
        }
        
        }
        outputFile << "\n" <<endl;
    }
    
    
    outputFile.close();
    cout << "Count output to text file" << endl;
    

}
