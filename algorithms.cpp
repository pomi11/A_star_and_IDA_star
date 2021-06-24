#include "algorithms.h"
#include <algorithm>


static int Anodes =0;
static int nodes = 0;

int A(AbstractState *begin, int index, int *result_g, std::vector<AbstractState*> *path, int *numNodes)
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
    f.insert(std::pair<AbstractState*,int>(begin,begin->get_h(index)));
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
        Anodes++; //liczenie odwiedzonych node'ów

    /*
     * jeśli n jest węzłem końvowym
     * to zakończ oraz zwróć ścieżkę s do n i g(n)
     */
        if(current->is_end())
        {
            *numNodes = Anodes;
            Anodes = 0;
            if(result_g!=nullptr)
                *result_g = gc;
            if(path!=nullptr)
                *path = PATH[current];

            g.clear();
            f.clear();
            PATH.clear();
            tmp.clear();
            OPEN.clear();
            CLOSE.clear();
            return 0;
        }

        std::map<AbstractState*,int> *nexts = current->get_nexts();

        bool is_in_OPEN = false;
        bool is_in_CLOSE = false;

    /*
     * petla po nastepnikach
     * obliczamy dla nich kosz dotarcia(g)
     */
        for(auto itr=nexts->begin();itr!=nexts->end();itr++)
        {
            is_in_OPEN = false;
            is_in_CLOSE = false;
            std::vector<AbstractState*>::iterator same;
            g_n_tmp = gc+itr->second;
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
                f_n_tmp = g_n_tmp+itr->first->get_h(index);
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
                    f_n_tmp = g_n_tmp+itr->first->get_h(index);
                    f[itr->first]=f_n_tmp;
                    if(is_in_OPEN)
                        OPEN.erase(same);
                    if(is_in_CLOSE)
                        CLOSE.erase(same);
                    OPEN.push_back(itr->first);
                    std::vector<AbstractState*> ttmp =tmp;
                    ttmp.push_back(itr->first);
                    PATH.insert(std::pair<AbstractState*,std::vector<AbstractState*>>(itr->first,ttmp));
                }
            }
        }
    }
    g.clear();
    f.clear();
    PATH.clear();
    tmp.clear();
    OPEN.clear();
    CLOSE.clear();

    return -1; //zwróć -1 jeśli nie znaleziono ścieżki
}

int A_no_nodes(AbstractState *begin,int index, int *result_g, std::vector<AbstractState*> *path)
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
    f.insert(std::pair<AbstractState*,int>(begin,begin->get_h(index)));
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
    /*
     * jeśli n jest węzłem końvowym
     * to zakończ oraz zwróć ścieżkę s do n i g(n)
     */
        if(current->is_end())
        {
            if(result_g!=nullptr)
                *result_g = gc;
            if(path!=nullptr)
                *path = PATH[current];

            g.clear();
            f.clear();
            PATH.clear();
            tmp.clear();
            OPEN.clear();
            CLOSE.clear();
            return 0; //zwróć 0 - znaleziono rozwiązanie
        }

        std::map<AbstractState*,int> *nexts = current->get_nexts();

        bool is_in_OPEN = false;
        bool is_in_CLOSE = false;

    /*
     * petla po nastepnikach
     * obliczamy dla nich kosz dotarcia(g)
     */
        for(auto itr=nexts->begin();itr!=nexts->end();itr++)
        {
            is_in_OPEN = false;
            is_in_CLOSE = false;
            std::vector<AbstractState*>::iterator same;
            g_n_tmp = gc+itr->second;

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
                f_n_tmp = g_n_tmp+itr->first->get_h(index);
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
                    f_n_tmp = g_n_tmp+itr->first->get_h(index);
                    f[itr->first]=f_n_tmp;
                    if(is_in_OPEN)
                        OPEN.erase(same);
                    if(is_in_CLOSE)
                        CLOSE.erase(same);
                    OPEN.push_back(itr->first);
                    std::vector<AbstractState*> ttmp =tmp;
                    ttmp.push_back(itr->first);
                    PATH.insert(std::pair<AbstractState*,std::vector<AbstractState*>>(itr->first,ttmp));
                }
            }
        }
    }
    g.clear();
    f.clear();
    PATH.clear();
    tmp.clear();
    OPEN.clear();
    CLOSE.clear();

    return -1; //zwróć -1 jeśli nie znaleziono ścieżki
}

int IDA(AbstractState *begin,int index, int *result_g, std::vector<AbstractState*> *path,int *numNodes)
{
    nodes = 0;
    int f_b = begin->get_h(index); //ustawiamy na początek limit=h(begin)
    std::vector<AbstractState*> PATH;
    PATH.push_back(begin);
    long long int t = 0;
    //long long int min = 999;
    bool isFound = false;
    int g = 0;
    int c = 0;

    while(1)
    {
        /*
         * Wykonujemy przeszukanie w głąb, z limitem f_b
         * PATH - ścieżka
         * index - która heurestyka, domyślne index=0
         * g - obecny koszt dojśia do node'a
         * f_b - obecny limit
         * t - zmienna, do której zapisany zostanie nowy limit do przeszukania w głąb
         * c - zmienna, do której zapisany zostanie koszt ścieżki do rozwiązania
         */
        isFound = IDASearch(PATH,index,g,f_b,&t,nullptr,&c);
        if(isFound)
        {
            if(result_g!=nullptr)
                *result_g = c;
            if(path!=nullptr)
                *path = PATH;

            PATH.clear();

            *numNodes = nodes;
            nodes = 0;

            return 0; // zwróć 0, znaleziono rozwiązanie
        }

        if(t>=depth)
        {
            //std::cout<<"error";
            return -1; // nie znaleziono rozwiązania
        }

        f_b = t;
    }

    return -1; // nie znaleziono rozwiązania
}

bool IDASearch(std::vector<AbstractState*> &path,int index, int g, int f_b, long long int *t, long long int *min, int *g_end)
{

    //pobieramy obecny node, który znajduje się na końcu path
    AbstractState *current = path.back();

    nodes++;
    int f_n = g+current->get_h(index); // obliczanie wartości f(n)
    bool isFound = false;

   /*
    * jeśli wartość f(n) dla obecnego node'a jest większa od limitu,
    * zwróć f_n.
    * Zostanie to wykorzystane w późniejszej części do znalezienia najmniejszej wartości
    * funkcji f(n) i powrotu do roota.
    */
    if(f_n>f_b)
    {
        *t = f_n;
        return false;
    }

    //znalezione rozwiązane - zwraca true
    if(current->is_end())
        return true;

    /*
     * wartość początkowa zmiennej pomocniczej
     * do znalezienia najmniejszej wartości f(n)
     * potrzebnej do ustalenia nowego limitu
     */
    long long int Mmin = depth;
    min = &Mmin;

    //szukamy następników
    std::map<AbstractState*,int> *nexts = current->get_nexts();
    for(auto itr=nexts->begin();itr!=nexts->end();itr++)
    {
        bool inPath = false;
        /*
         * sprawdzamy, czy nowy node jest w ścieżce
         * jeśli jest, to nie robimy obliczeń, ponieważ doprowadziłoby to do nieskończonej pętli
         */
        for(auto vitr=path.begin();vitr!=path.end();vitr++)
        {
            if(itr->first==(*vitr.base()))
            {
                inPath=true;
                break;
            }
        }

        /*
         * jak nowy node nie jest w obecnej ścieżce
         * wykonujemy dalsze przeszukanie
         */
        if(!inPath)
        {
            /*
             * wrzucamy następny node na stos
             */
            path.push_back(itr->first);

            /*
             * ustawiamy ostateczny koszt ścieżki, jeśli
             * znaleziono rozwiązanie
             */
            *g_end = g+itr->second;

            /*
             * wykonujemy przeszukanie nowego node'a
             */
            isFound = IDASearch(path,index,g+itr->second,f_b,t,min,g_end);
            if(isFound)
            {


                return isFound;
            }
            /*
             * brak rozwiązania, jeśli zwrócony limit jest mniejszy
             * od min, to ustaw nową wartość min
             * w ten sposób znajdziemy najmniejszą wartosć, do której
             * możemy zwiększyć limit
             */
            if(*t<*min)
                *min = *t;
            /*
             * jeśli nie znaleziono rozwiązania, wracamy
             * i zdejmujemy ze stosu ostatniego node'a
             */
            path.pop_back();
        }
    }

    return false;
}

int IDA_no_nodes(AbstractState *begin,int index, int *result_g, std::vector<AbstractState*> *path)
{

    int f_b = begin->get_h(index);//ustawiamy na początek limit=h(begin)
    std::vector<AbstractState*> PATH;
    PATH.push_back(begin);
    long long int t = 0;
    //long long int min = 999;
    bool isFound = false;
    int g = 0;
    int c = 0;
    while(1)
    {
        /*
         * Wykonujemy przeszukanie w głąb, z limitem f_b
         * PATH - ścieżka
         * index - która heurestyka, domyślne index=0
         * g - obecny koszt dojśia do node'a
         * f_b - obecny limit
         * t - zmienna, do której zapisany zostanie nowy limit do przeszukania w głąb
         * c - zmienna, do której zapisany zostanie koszt ścieżki do rozwiązania
         */
        isFound = IDASearch_no_nodes(PATH,index,g,f_b,&t,nullptr,&c);
        if(isFound)
        {
            if(result_g!=nullptr)
                *result_g = c;
            if(path!=nullptr)
                *path = PATH;

            PATH.clear();
            return 0; // zwróć 0, znaleziono rozwiązanie
        }

        if(t>=depth)
        {
            //std::cout<<"error";
            return -1; // nie znaleziono rozwiązania
        }
        /*if(t>100)
            std::cout<<"t="<<t<<"\n";*/
        f_b = t;
    }

    return -1; // nie znaleziono rozwiązania
}

bool IDASearch_no_nodes(std::vector<AbstractState*> &path,int index, int g, int f_b, long long int *t, long long int *min, int *g_end)
{
    //pobieramy obecny node, który znajduje się na końcu path
    AbstractState *current = path.back();

    int f_n = g+current->get_h(index); // obliczanie wartości f(n)
    bool isFound = false;

    /*
     * jeśli wartość f(n) dla obecnego node'a jest większa od limitu,
     * zwróć f_n.
     * Zostanie to wykorzystane w późniejszej części do znalezienia najmniejszej wartości
     * funkcji f(n) i powrotu do roota.
     */
    if(f_n>f_b)
    {
        *t = f_n;
        return false;
    }

    //znalezione rozwiązane - zwraca true
    if(current->is_end())
        return true;

    /*
     * wartość początkowa zmiennej pomocniczej
     * do znalezienia najmniejszej wartości f(n)
     * potrzebnej do ustalenia nowego limitu
     */
    long long int Mmin = depth;
    min = &Mmin;

    //szukamy następników
    std::map<AbstractState*,int> *nexts = current->get_nexts();
    for(auto itr=nexts->begin();itr!=nexts->end();itr++)
    {
        bool inPath = false;
        /*
         * sprawdzamy, czy nowy node jest w ścieżce
         * jeśli jest, to nie robimy obliczeń, ponieważ doprowadziłoby to do nieskończonej pętli
         */
        for(auto vitr=path.begin();vitr!=path.end();vitr++)
        {
            if(itr->first==(*vitr.base()))
            {
                inPath=true;
                break;
            }
        }
        /*
         * jak nowy node nie jest w obecnej ścieżce
         * wykonujemy dalsze przeszukanie
         */
        if(!inPath)
        {
            /*
             * ustawiamy ostateczny koszt ścieżki, jeśli
             * znaleziono rozwiązanie
             */
            *g_end = g+itr->second;
            /*
             * wrzucamy następny node na stos
             */
            path.push_back(itr->first);

            isFound = IDASearch_no_nodes(path,index,g+itr->second,f_b,t,min,g_end);
            if(isFound)
            {
                return isFound;
            }
            /*
             * brak rozwiązania, jeśli zwrócony limit jest mniejszy
             * od min, to ustaw nową wartość min
             * w ten sposób znajdziemy najmniejszą wartosć, do której
             * możemy zwiększyć limit
             */
            if(*t<*min)
                *min = *t;

            /*
             * jeśli nie znaleziono rozwiązania, wracamy
             * i zdejmujemy ze stosu ostatniego node'a
             */
            path.pop_back();
        }
    }
    return false;
}
