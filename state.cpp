#include "state.h"
#include <iostream>

State::State()
{

}

State::State(int column_no, std::string row,bool is_end,bool is_start)
{
    std::string tmp = get_name();
    tmp +=row;
    tmp +=std::to_string(column_no);
    set_name(tmp);

    this->column=column_no;
    this->row=row;
    this->start=is_start;
    this->end=is_end;
}

void State::set_name(std::string name)
{
    this->name=name;
}

std::string State::get_name()
{
    return name;
}

void State::set_end(bool is_end)
{
    this->end=is_end;
}

void State::set_start(bool is_start)
{
    this->start=is_start;
}

bool State::is_start()
{
    return start;
}
bool State::is_end()
{
    return end;
}

void State::add_map(AbstractState* state, int value)
{
    this->actions.insert(std::pair<AbstractState*,int>(state,value));
}

std::map<AbstractState*,int> *State::get_nexts()
{
    return &actions;
}

int State::calculate_cost(AbstractState *toState)
{
    int c =0;
    for(auto itr = actions.begin();itr!=actions.end();itr++)
    {
        if(itr->first->get_name()==toState->get_name())
            return itr->second;
    }
    return c;
}

void State::set_cost(int value)
{
    this->cost=value;
}

int State::get_cost()
{
    return cost;
}

int State::get_h(int index)
{
    return h[index];
}

void State::set_h(int value,size_t index)
{
    if(h.size()==0 || index>h.size()-1)
        h.push_back(value);
    else
        this->h[index]=value;
}

std::string State::getRow()
{
    return row;
}

int State::getColumn()
{
    return column;
}

int State::get_number_of_h()
{
    return this->h.size();
}
