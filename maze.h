#ifndef ACTIONS_H
#define ACTIONS_H

#include <iostream>
#include <state.h>
#include <vector>

class Maze
{
private:
    std::string area="";
    std::vector<State> states;
    State *startState;// = nullptr;
    State *endState;// = nullptr;
    int n = 0;
    char wallChar = 'X';
    char startChar = 'e';
    char endChar = 'j';
    int defaultCost = 1, endCost = 1, startCost = 1;
    std::string fileName;

    std::vector<std::vector<State>> states_with_H;

public:
    Maze();
    Maze(std::string area,std::vector<State> states = std::vector<State>());

    void set_state(std::vector<State> Nstates);
    void add_state(State state);

    void set_n(int value);
    int get_n();

    std::string getArea();
    std::vector<State> *getStates();
    State* getStartState();
    State* getEndState();

    State* findByAdress(std::string adress);

    void setWallChar(char c);
    void setStartChar(char c);
    void setEndChar(char c);
    void setDefCost(int c);
    void setStartCost(int c);
    void setEndCost(int c);

    char getWallChar();
    char getStartChar();
    char getEndChar();
    int getDefCost();
    int getStartCost();
    int getEndCost();

    void set_file_name(std::string name);
    std::string get_file_name();


    int generate_H(int (*heurestic_function)(State,Maze*));


};

#endif // ACTIONS_H
