#ifndef STAN_H
#define STAN_H

#include <map>
#include <vector>

class AbstractState
{
protected:
    std::string name;
    std::vector<int> h;
    std::map<AbstractState*,int> actions;
    std::vector<AbstractState*> parents;
    bool end=false, start=false;

public:
    virtual void set_name(std::string name) =0;
    virtual std::string get_name()=0;

    virtual int calculate_cost(AbstractState *toState)=0;

    virtual void set_h(int value, size_t index=0)=0;
    virtual int get_h(int index =0)=0;

    virtual void add_map(AbstractState* state, int value)=0;

    virtual std::map<AbstractState*,int> *get_nexts()=0;

    virtual bool is_start()=0;
    virtual bool is_end()=0;

    virtual void set_end(bool is_end=true)=0;
    virtual void set_start(bool is_start=true)=0;

    virtual int get_number_of_h()=0;
};

class State : public AbstractState
{
private:
    int column = -1;
    std::string row="/";
    int cost =1;

public:
    State();
    State(int column_no, std::string row,bool is_end=false,bool is_start=false);

    std::string getRow();
    int getColumn();

    void set_cost(int value);
    int get_cost();

    void set_name(std::string name);
    std::string get_name();

    int calculate_cost(AbstractState *toState);

    void set_h(int value, size_t index=0);
    int get_h(int index =0);

    void add_map(AbstractState* state, int value);

    std::map<AbstractState*,int> *get_nexts();

    bool is_start();
    bool is_end();

    void set_end(bool is_end=true);
    void set_start(bool is_start=true);

    int get_number_of_h();

};

#endif // STAN_H
