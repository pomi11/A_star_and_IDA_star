#ifndef SUMMARY_H
#define SUMMARY_H

#include "maze.h"
#include <chrono>
class Summary
{
private:
    std::vector<Maze> *mazes;
    std::map<std::string,std::map<int,int>> time;
    std::map<std::string,std::map<int,int>> pathLength;
    std::map<std::string,std::map<int,int>> cost;
    std::map<std::string,std::map<int,int>> nodes;
    std::map<std::string,std::map<int,std::vector<AbstractState*>>> path;
    int repeats;

    std::string outputDir;
    //std::string headPaths,headStates_H,headSummary,headResults;
public:
    Summary();
    Summary(std::vector<Maze> *mazes);
    Summary(std::vector<Maze> *mazes, const std::string &outputDir);

    int run(int (*algorithm)(AbstractState*, int, int*, std::vector<AbstractState*>*, int *),int (*algorithm_no_nodes)(AbstractState *, int, int *, std::vector<AbstractState *> *),
            std::string alg, int mazeNum, int hNum, bool nodes = true);

    int get_cost(std::string alg,int mazeNum);
    int get_pathLenght(std::string alg,int mazeNum);
    int get_nodes(std::string alg,int mazeNum);
    int get_time(std::string alg,int mazeNum);

    std::string get_results_head(std::string fileName);
    std::string get_paths_head(std::string alg_name, int mazeNum);
    //int add(std::string alg, int h, int value);
    int add(std::string alg, int mazeNum, std::vector<AbstractState*> path);

    int save_summary(bool show_in_cmd =false);

    std::string get_path_string(std::string alg, int mazeNum);
    std::vector<AbstractState*> get_path(std::string alg, int mazeNum);

    void set_repeats(int value);

    std::string add_spaces(int value);
};


#endif // SUMMARY_H
