#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "state.h"
#include <vector>
#include <iostream>

extern long long int depth;

int A(AbstractState *begin, int index = 0, int *result_g = nullptr, std::vector<AbstractState*> *path = nullptr,int *numNodes = nullptr);
int IDA(AbstractState *begin, int index = 0, int *result_g = nullptr, std::vector<AbstractState*> *path = nullptr,int *numNodes = nullptr);

int A_no_nodes(AbstractState *begin, int index = 0, int *result_g = nullptr, std::vector<AbstractState*> *path = nullptr);
int IDA_no_nodes(AbstractState *begin, int index = 0, int *result_g = nullptr, std::vector<AbstractState*> *path = nullptr);

bool IDASearch(std::vector<AbstractState*> &path,int index,int g, int f_b, long long int *t, long long int *min, int *g_end);
bool IDASearch_no_nodes(std::vector<AbstractState*> &path,int index,int g, int f_b, long long int *t, long long int *min, int *g_end);

#endif // ALGORITHMS_H
