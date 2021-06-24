#include "algorithms.h"
#include <algorithm>
static int Anodes =0;

void A(AbstractState *begin, AbstractState *end)
{
    AbstractState *current = nullptr;
    int gc=0;

    std::map<AbstractState*,int> f,g;
    std::vector<AbstractState*> OPEN, CLOSE;

    std::map<AbstractState*,std::vector<AbstractState*>> PATH;

    current = begin;

    /*
     * pobranie wezla startowego
     * ustawienie f(s) i g(s)
     */
    OPEN.push_back(begin);
    std::vector<AbstractState*> tmp;
    tmp.push_back(begin);
    PATH.insert(std::pair<AbstractState*,std::vector<AbstractState*>>(begin,tmp));
    f.insert(std::pair<AbstractState*,int>(begin,0));
    g.insert(std::pair<AbstractState*,int>(begin,0));

    auto itr_tmp = OPEN.begin();
    int f_n_tmp = 0;
    int g_n_tmp = 0;
    int f_n_tmp_min=0;

    while(1)
    {
        /*
     * Poszukiwanie węzła n o najmniejszej wartości f(n)
     */

        for(auto itr=OPEN.begin();itr!=OPEN.end();itr++)
        {
            //g_n_tmp = gc+current->calculate_cost(*itr.base());
            f_n_tmp = f[*itr.base()];

            if((f_n_tmp<f_n_tmp_min) || itr == OPEN.begin())
            {
                f_n_tmp_min = f_n_tmp;
                current = *itr.base();
                itr_tmp = itr;
            }
        }
        gc = g[current];
        OPEN.erase(itr_tmp);
        CLOSE.push_back(current);
        tmp = PATH[current];
        //Anodes++;

        /*
     * jeśli n jest węzłem końvowym
     * to zakończ oraz zwróć ścieżkę s do n i g(n)
     */
        if(current==end)
        {
            //std::cout<<"\nhappy?\ng="<<gc<<"\n";
            //std::cout<<Anodes<<std::endl;
            for(int i=0; i<PATH[end].size();i++)
            {
                std::cout<<i<<PATH[end][i]->get_name()<<"\n";
            }
            //std::cout<<std::endl;
            //Anodes = 0;
            break;
            //zwróć ścieżkę s do n i g(n)
        }

        std::map<AbstractState*,int> nexts = current->getMap();

        bool is_in_OPEN = false;
        bool is_in_CLOSE = false;

        /*
     * petla po nastepnikach
     * obliczamy dla nich kosz dotarcia(g)
     */
        for(auto itr=nexts.begin();itr!=nexts.end();itr++)
        {
            is_in_OPEN = false;
            is_in_CLOSE = false;
            std::vector<AbstractState*>::iterator same;
            g_n_tmp = gc+itr->second;
            //g.insert(std::pair<AbstractState,int>(*itr->first,g_n_tmp));

            /*
         * sprawdzamy czy n należy do zbioru OPEN
         */
            for(auto OPEN_itr=OPEN.begin();OPEN_itr!=OPEN.end();OPEN_itr++)
            {
                if(*OPEN_itr.base()==itr->first)
                {
                    is_in_OPEN=true;
                    same = OPEN_itr;
                    break;
                }
            }

            /*
         * sprawdzamy czy n należy do zbioru CLOSE
         */
            for(auto CLOSE_itr=CLOSE.begin();CLOSE_itr!=CLOSE.end();CLOSE_itr++)
            {
                if(*CLOSE_itr.base()==itr->first)
                {
                    is_in_CLOSE=true;
                    same = CLOSE_itr;
                    break;
                }
            }
        /*
         * jeśli n nie należy do zbioru OPEN ani do CLOSE
         * dodajemy go do zbioru OPEN
         * ustawiamy koszt dotarcia g(n) oraz wartość funkcji f(n)
         */
            if((!is_in_OPEN) && (!is_in_CLOSE))
            {
                OPEN.push_back(itr->first);
                g.insert(std::pair<AbstractState*,int>(itr->first,g_n_tmp));
                f_n_tmp = g_n_tmp+itr->first->get_h();
                f.insert(std::pair<AbstractState*,int>(itr->first,f_n_tmp));
                std::vector<AbstractState*> ttmp =tmp;
                ttmp.push_back(itr->first);
                PATH.insert(std::pair<AbstractState*,std::vector<AbstractState*>>(itr->first,ttmp));
            }

            if(is_in_OPEN || is_in_CLOSE)
            {
                int g_m = g[*same.base()];
                if(g_m>g_n_tmp)
                {
                    PATH.erase(*same.base());
                    g[itr->first]=g_n_tmp;
                    f_n_tmp = g_n_tmp+itr->first->get_h();
                    f[itr->first]=f_n_tmp;
                    if(is_in_OPEN)
                        OPEN.erase(same);
                    if(is_in_CLOSE)
                        CLOSE.erase(same);
                    OPEN.push_back(itr->first);
                }
            }
        }
        if(OPEN.empty())
        {
            std::cout<<"ss";
        }
    }
}
static int nodes =0;

void IDA(AbstractState *begin, AbstractState *end)
{

    int f_b = begin->get_h();
    std::vector<AbstractState*> PATH;
    PATH.push_back(begin);
    long long int t = 0;
    long long int min = 999999999999999999;
    bool isFound = false;
    int g = 0;
    int c = 0;
    while(1)
    {
        isFound = IDASearch(PATH,end,g,f_b,&t,&min,&c);
        if(isFound)
            break;
        if(t==999999999999999999)
        {
            std::cout<<"error";
            break;
        }
        f_b = t;
    }

    //std::cout<<"g="<<c<<std::endl;
    //std::cout<<nodes<<std::endl;
    for(int i=0; i<PATH.size();i++)
    {
        std::cout<<i<<PATH[i]->get_name()<<"\n";
    }
    //nodes = 0;
}

bool IDASearch(std::vector<AbstractState*> &path,AbstractState *end, int g, int f_b, long long int *t, long long int *min, int *_g)
{
    AbstractState *current = path.back();
    //nodes++;
    int f_n = g+current->get_h();
    bool isFound = false;

    if(f_n>f_b)
    {
        *t = f_n;
        return false;
    }
    if(current==end)
        return true;

    //long long int min = 999999999999999999;
    std::map<AbstractState*,int> nexts = current->getMap();
    for(auto itr=nexts.begin();itr!=nexts.end();itr++)
    {
        bool inPath = false;
        for(auto vitr=path.begin();vitr!=path.end();vitr++)
        {
            if(itr->first->get_name()==(*vitr.base())->get_name())
            {
                inPath=true;
                break;
            }
        }
        if(!inPath)
        {
            path.push_back(itr->first);
            *_g = g+itr->second;
            isFound = IDASearch(path,end,g+itr->second,f_b,t,min,_g);
            if(isFound)
                return isFound;
            if(*t<*min)
                *min = *t;
            path.pop_back();
        }
    }
    return false;
}
