#include "maze.h"

Maze::Maze()
{

}

Maze::Maze(std::string area,std::vector<State> states)
{
    this->area=area;
    this->states=states;
}

void Maze::set_state(std::vector<State> Nstates)
{
    for(size_t i=0;i<Nstates.size();i++)
    {
        states.push_back(Nstates[i]);
        if(states[i].is_end())
            this->endState=&states[i];

        if(states[i].is_start())
            this->startState=&states[i];
    }
}

void Maze::add_state(State state)
{
    this->states.push_back(state);
}

std::string Maze::getArea()
{
    return area;
}

std::vector<State> *Maze::getStates()
{
    return &states;
}

State* Maze::findByAdress(std::string adress)
{
    if(adress.length()==0)
    {
        std::cout<<"Brak adresu\n";
        return nullptr;
    }

    std::string tmp,tmp2;
    int last_letter =0;

    for(size_t i=0;i<adress.length();i++)
    {
        if(adress[i]>64 && adress[i]<91)
        {
            tmp+=adress[i];
            last_letter++;
        }
    }

    for(size_t i=last_letter;i<adress.length();i++)
        tmp2 +=adress[i];

    int column= atoi(tmp2.c_str());

    if((column<=0))
    {
        std::cout<<"Bledny adres\n";
        return nullptr;
    }

    for(size_t i=0;i<states.size();i++)
    {
         if(states[i].get_name()==adress)
            return &states[i];
    }

    return nullptr;
}

State* Maze::getStartState()
{
    State *s= nullptr;
    for(auto itr = states.begin();itr!=states.end();itr++)
    {
        if(itr.base()->is_start())
            s = itr.base();
    }

    return s;
}
State* Maze::getEndState()
{
    State *s = nullptr;
    for(auto itr = states.begin();itr!=states.end();itr++)
    {
        if(itr.base()->is_end())
            s = itr.base();
    }

    return s;
}

void Maze::set_n(int value)
{
    this->n=value;
}

int Maze::get_n()
{
    return n;
}

void Maze::setWallChar(char c)
{
    this->wallChar=c;
}
void Maze::setStartChar(char c)
{
    this->startChar=c;
}
void Maze::setEndChar(char c)
{
    this->endChar=c;
}

char Maze::getWallChar()
{
    return wallChar;
}
char Maze::getStartChar()
{
    return startChar;
}
char Maze::getEndChar()
{
    return endChar;
}

int Maze::getDefCost()
{
    return defaultCost;
}

int Maze::getStartCost()
{
    return startCost;
}

int Maze::getEndCost()
{
    return endCost;
}

void Maze::setDefCost(int c)
{
    this->defaultCost=c;
}

void Maze::setStartCost(int c)
{
    this->startCost=c;
}

void Maze::setEndCost(int c)
{
    this->endCost=c;
}

void Maze::set_file_name(std::string name)
{
    this->fileName=name;
}

std::string Maze::get_file_name()
{
    return fileName;
}

int Maze::generate_H(int (*heurestic_function)(State,Maze*))
{
    int idx = this->getStartState()->get_number_of_h();
    int index = -1;
        State end;
        for(size_t j = 0;j<this->getStates()->size();j++)
        {
            if(this->getStates()->at(j).is_start())
            {
                index = j;
                break;
            }
        }

        for(auto itr = this->getStates()->begin();itr!=this->getStates()->end();itr++)
            itr->set_h(heurestic_function(*itr,this),idx);

    if(index<=-1)
    {
        std::cout<<"Cos poszlo nie tak! Brak poczatku w labiryncie!\n";
        return -1;
    }

    return idx;
}
