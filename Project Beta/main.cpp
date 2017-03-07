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
    int xpos ;
    int ypos ;
    int ix ;
    int iy ;
    int dx;
    int dy;
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
    void sense2(grid);
    void update(qlearner *pqt, grid);
    void reset();
    int choice2();
};

class body{
public:
    int xpos;
    int ypos;
    
};


void Qsetup(qlearner* pqt){
    int num_state=100;
    int num_actions=4;
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
            pqt->Q.at(i).at(j)=.001*LYRAND;
        }
    }

    
}

void Qsetup2(qlearner* pqt){
    int num_state=400;
    int num_actions=4;
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
            pqt->Q.at(i).at(j)=.001*LYRAND;
        }
    }
    
    
}


int agent::choice2(){//0 up 1 down 2 left 3 right
    int input=rand()%4;
    double epsilon=.05;
    if(LYRAND > epsilon){
    if(dx < 0){ // robot is left of goal
        input = 3;
    }
    if(dx > 0){ // robot is right of goal
        input = 2;
    }
    if(dy < 0){ // robot is below goal
        input = 0;
    }
    if(dy > 0){ // robot is above goal
        input = 1;
    }
    }
    return input;
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

void agent::sense(grid world){
    pstate=state;
    state = ypos*world.xmax+xpos;
    //cout<<state<<"\t"<<pstate<<endl;
}

void agent::sense2(grid world){
    pstate=state;
    dx=xpos-world.goalx;
    dy=ypos-world.goaly;
    if(dx>0 & dy > 0){
        state=dy*world.xmax+dx;
    }
    if(dx>0 & dy<0){
        state=100+(-1*dy*world.xmax+dx);
    }
    if(dx<0 & dy >0){
        state=200+(dy*world.xmax+dx*-1);
    }
    if(dx<0 & dy <0){
        state=300+(-1*dy*world.xmax+dx*-1);
    }
    //state = ypos*world.xmax+xpos;
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
void learningCurve(int x, int y, int episodes){
    agent robot;
    grid world;
    qlearner qt;
    qlearner* pqt;
    pqt = &qt;
    Qsetup(pqt);
    world.xmax=x-1;
    world.ymax=y-1;
    world.goalx=4;
    world.goaly=8;
    
    int stat_runs=30;
    
    ofstream outputFile;
    outputFile.open("LCout.txt");
    int counting;
    for(int s=0; s<stat_runs;s++){
        Qsetup(pqt);  //resets Q table for each stat run to generate a learning curve over the # of episodes
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
    cout << "Learning Curve output to text file" << endl;
}

void testD(int x, int y, int episodes){
    agent robot;
    grid world;
    qlearner qt;
    qlearner* pqt;
    pqt = &qt;
    Qsetup(pqt);
    world.xmax=x-1;
    world.ymax=y-1;
    world.goalx=4;
    world.goaly=8;
    
    int stat_runs=30;
    
    for(int s=0; s<stat_runs;s++){
        Qsetup(pqt);
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
        }
    }
    
    for(int h=0;h<100;h++){
        for(int g=0; g<4;g++){
            assert(pqt->Q.at(h).at(g) <=100);
        }
    }
    cout<<"Test D passed"<<endl;
}

void agent::reset(){
    xpos = ix;
    ypos = iy;
    win= false;
}

void testE(int x, int y, int episodes){
    agent robot;
    grid world;
    qlearner qt;
    qlearner* pqt;
    pqt = &qt;
    Qsetup(pqt);
    world.xmax=x-1;
    world.ymax=y-1;
    world.goalx=4;
    world.goaly=8;
    robot.ix=2;
    robot.iy=1;
    int stat_runs=30;
    
    for(int s=0; s<stat_runs;s++){
        Qsetup(pqt);
        for(int c=0; c<episodes; c++){
            //robot.xpos=robot.ix;
            //robot.ypos=robot.iy;
            //robot.win=false;
            robot.reset();
            assert(robot.xpos == robot.ix & robot.ypos == robot.iy & not(robot.win));
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
        }
    }
    cout << "test E passed" << endl;
    
}
void testF(int x, int y, int episodes){
    agent robot;
    grid world;
    qlearner qt;
    qlearner* pqt;
    pqt = &qt;
    Qsetup(pqt);
    world.xmax=x-1;
    world.ymax=y-1;
    world.goalx=4;
    world.goaly=8;
    
    int stat_runs=30;
    
    int counting;
    int icount = 0;
    int fcount=0;
    for(int s=0; s<stat_runs;s++){
        Qsetup(pqt);
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
                if(s==0 & c==0){
                    icount=counting;
                }
                if(s==(stat_runs-1) & c==(episodes-1)){
                    fcount=counting;
                }
            }
            
        }
    }
    //cout << icount <<"\t"<< fcount << endl;
    assert(icount>fcount); // number of moves on initial run > number of moves on final run
    cout << "Test F passed" << endl;
}
void testG(int x, int y, int episodes){
    agent robot;
    grid world;
    qlearner qt;
    qlearner* pqt;
    pqt = &qt;
    Qsetup2(pqt);
    world.xmax=x-1;
    world.ymax=y-1;
    world.goalx=4;
    world.goaly=8;
    
    int stat_runs=30;
    
    ofstream outputFile;
    outputFile.open("Gout.txt");
    
    int counting;
    int icount = 0;
    int fcount=0;
    for(int s=0; s<stat_runs;s++){
        Qsetup2(pqt);
        for(int c=0; c<episodes; c++){
            robot.xpos=1;
            robot.ypos=1;
            robot.win=false;
            int counting = 0;
            bool notwinner=true;
            while(notwinner != robot.win){
                robot.check(world);
                robot.eval(world);
                robot.sense2(world);
                robot.input = robot.choice2();
                robot.move(world);
                robot.eval(world);
                robot.sense2(world);
                robot.update(pqt, world);
                robot.check(world);
                robot.eval(world);
                counting++;
                if(s==0 & c==0){
                    icount=counting;
                }
                if(s==(stat_runs-1) & c==(episodes-1)){
                    fcount=counting;
                }
            }
            if(outputFile.is_open()){
                outputFile << counting << "\t";
            }
     
        }
        outputFile << "\n" <<endl;
    }
    outputFile.close();
    cout << "TestG output to text file" << endl;
    
}

int main(){
    cout << "main start" << endl;
    srand(time(NULL));
    int episodes=500;
    int x=10;
    int y=10;
    learningCurve(x, y, episodes);
    testD(x, y, episodes);
    testE(x,y,episodes);
    testF(x,y,episodes);
    testG(x,y,episodes);
}
