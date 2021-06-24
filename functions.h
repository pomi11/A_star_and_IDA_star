#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <dirent.h>
#include <fstream>
#include <maze.h>
#include <math.h>

std::vector<Maze> load_files(const std::string &inputDir, std::map<std::string,char> &settings);
void load_settings(std::map<std::string,char> &settings);
void generate_states(std::vector<Maze> &mazes);
void generate_actions(std::vector<Maze> &mazes);

int load_arguments(int argc,char *argv[],std::string &inputDir, std::string &outputDir, int &repeats, long long int &depth);

int H1(State s,Maze *temp);
int H2(State s,Maze *temp);

bool is_number(std::string c);

#endif // FUNCTIONS_H
