#include "summary.h"
#include <fstream>
#include <iomanip>

Summary::Summary()
{

}

Summary::Summary(std::vector<Maze> *mazes)
{
    this->mazes=mazes;
}

Summary::Summary(std::vector<Maze> *mazes, const std::string &outputDir)
{
    this->mazes=mazes;
    this->outputDir=outputDir;
}

int ::Summary::add(std::string alg,int mazeNum, std::vector<AbstractState*> path)
{
    std::map<int,std::vector<AbstractState*>> p;
    p.insert(std::pair<int,std::vector<AbstractState*>>(mazeNum,path));

    this->path.insert(std::pair<std::string, std::map<int,std::vector<AbstractState*>> >(alg,p));

    return 0;
}

std::string Summary::get_path_string(std::string alg, int mazeNum)
{
    std::string temp;
    std::vector<AbstractState*> pTemp = path[alg][mazeNum];
    for(size_t asd=0;asd<pTemp.size();asd++)
    {
        temp+=pTemp[asd]->get_name();
        if(asd!=pTemp.size()-1)
            temp+="-->";
    }

    return temp;
}

std::vector<AbstractState*> Summary::get_path(std::string alg,int mazeNum)
{
    return path[alg][mazeNum];
}

std::string Summary::get_paths_head(std::string alg_name, int mazeNum)
{
    std::string c;
    c+="======================================================================\n";
    c+="------Wygenerowana sciezka przez algorytm "+alg_name+"------------------------\n";
    c+="\n";
    c+=get_path_string(alg_name,mazeNum)+"\n";
    c+="\n";
    c+="======================================================================\n";

    return c;
}

std::string Summary::get_results_head(std::string fileName)
{
    std::string temp;
    temp+="======================================================================\n";
    temp+=fileName+"\n";
    temp+="liczba uruchomien: "+std::to_string(repeats)+"\n";
    temp+="----------------------------------------------------------------------\n";
    temp+="Algorytm | czas\t \t| node\'y | dl. sciezki\t | koszt dotarcia |\n";
    temp+="----------------------------------------------------------------------\n";

    return temp;
}

void Summary::set_repeats(int value)
{
    this->repeats=value;
}

std::string add_spaces(int value)
{
    std::string temp="";
    for(int i=0;i<value;i++)
        temp +=' ';

    return temp;
}

int Summary::run(int (*algorithm)(AbstractState *, int, int *, std::vector<AbstractState *> *,int*),
                 int (*algorithm_no_nodes)(AbstractState *, int, int *, std::vector<AbstractState *> *),
                 std::string alg, int mazeNum, int hNum, bool nodes)
{
    State *startState = this->mazes->at(mazeNum).getStartState();

    int n = 0;
    //int *numNodes = nullptr;
    int result_g = 0;
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration= std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
    int isFound = -1;

    /*if(nodes)
        numNodes = &n;*/

    int avg_time = 0;
    for(int xz=0;xz<repeats;xz++)
    {
        result_g = 0;
        start = std::chrono::high_resolution_clock::now();
        isFound = algorithm_no_nodes(startState,hNum,&result_g,&path[alg][mazeNum]);
        stop = std::chrono::high_resolution_clock::now();
        duration= std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);

        avg_time+=duration.count();
    }

    algorithm(startState,hNum,&result_g,&path[alg][mazeNum],&n);

    if(!nodes)
        time[alg][mazeNum]=avg_time/repeats;

    if(isFound==0)
    {
        pathLength[alg][mazeNum]=path[alg][mazeNum].size();
        cost[alg][mazeNum] = result_g;
        this->nodes[alg][mazeNum]=n;
    }
    else
    {
        pathLength[alg][mazeNum]=0;
        cost[alg][mazeNum] = 0;
        this->nodes[alg][mazeNum]=0;
    }

    return 0;
}

int Summary::get_cost(std::string alg,int mazeNum)
{
    return cost[alg][mazeNum];
}
int Summary::get_pathLenght(std::string alg,int mazeNum)
{
    return pathLength[alg][mazeNum];
}
int Summary::get_nodes(std::string alg,int mazeNum)
{
    return nodes[alg][mazeNum];
}
int Summary::get_time(std::string alg,int mazeNum)
{
    return time[alg][mazeNum];
}

int Summary::save_summary(bool show_in_cmd)
{
    int totalTime_A =0, totalTime_IDA=0,
            totalTime_A_H1=0,totalTime_A_H2=0,
            totalTime_IDA_H1=0,totalTime_IDA_H2=0;
    int totalNode_A =0, totalNode_IDA=0, totalCost_A=0, totalCost_IDA=0, totalLength_A=0, totalLength_IDA=0;
    int totalNode_A_H1=0, totalNode_IDA_H1=0, totalCost_A_H1=0, totalCost_IDA_H1=0, totalLength_A_H1=0, totalLength_IDA_H1=0;
    int totalNode_A_H2=0, totalNode_IDA_H2=0, totalCost_A_H2=0, totalCost_IDA_H2=0, totalLength_A_H2=0, totalLength_IDA_H2=0;
    int totalFails_A = 0, totalFails_IDA = 0, totalBestCnt_A = 0, totalBestCnt_IDA= 0, totalShortestCnt_A= 0, totalShortestCnt_IDA = 0;
    int totalFails_A_H1 = 0, totalFails_A_H2 = 0, totalBestCnt_A_H1 = 0, totalBestCnt_A_H2= 0, totalShortestCnt_A_H1= 0, totalShortestCnt_A_H2 = 0;
    int totalFails_IDA_H1 = 0, totalFails_IDA_H2 = 0, totalBestCnt_IDA_H1 = 0, totalBestCnt_IDA_H2= 0, totalShortestCnt_IDA_H1= 0, totalShortestCnt_IDA_H2 = 0;

    std::map<std::string,std::map<int,int>> totalTime_n;
    std::map<std::string,std::map<int,int>> totalLength_n;
    std::map<std::string,std::map<int,int>> totalCost_n;
    std::map<std::string,std::map<int,int>> totalNode_n;
    std::map<std::string,std::map<int,int>> fails_n;
    std::map<std::string,std::map<int,int>> bestCost_n;
    std::map<std::string,std::map<int,int>> shortestLength_n;

    std::vector<int> list_of_n;

    std::vector<std::string> algorithms;
    algorithms.push_back("A_H1");
    algorithms.push_back("A_H2");
    algorithms.push_back("IDA_H1");
    algorithms.push_back("IDA_H2");

    std::map<int,int> shortestTmp, bestTmp;

    std::map<int,int> number_of_mazes_n;

    for(size_t i =0;i<mazes->size();i++)
    {
        int n = mazes->at(i).get_n();
        bool found = false;
        for(size_t k = 0;k<list_of_n.size();k++)
        {
            if(list_of_n[k]==n)
            {
                found = true;
                number_of_mazes_n[n]++;
                break;
            }
        }

        if(found==false)
        {
            list_of_n.push_back(n);
            number_of_mazes_n[n] = 1;
        }

        for(auto itr = algorithms.begin();itr!=algorithms.end();itr++)
        {
            std::string temp = *itr.base();

            if(number_of_mazes_n[n]==1)
                fails_n[temp][n] = 0;

            if(cost[temp][i]==0)
                fails_n[temp][n]+=1;

            if(cost[temp][i]!=0)
            {
                totalTime_n[temp][n]+=time[temp][i];
                totalLength_n[temp][n]+=pathLength[temp][i];
                totalCost_n[temp][n]+=cost[temp][i];
                totalNode_n[temp][n]+=nodes[temp][i];
            }

            if(itr == algorithms.begin())
            {
                shortestTmp[i]=pathLength[temp][i];
                bestTmp[i] = cost[temp][i];
            }

            bestCost_n[temp][n]=0;
            shortestLength_n[temp][n]=0;

            if(pathLength[temp][i]!=0)
            {
                if(pathLength[temp][i]<shortestTmp[i])
                shortestTmp[i] = pathLength[temp][i];
            }

            if(cost[temp][i]!=0)
            {
                if(cost[temp][i]<bestTmp[i])
                bestTmp[i] = cost[temp][i];
            }
        }
    }

    for(size_t k = 0;k<mazes->size();k++)
    {
        int n = mazes->at(k).get_n();

        for(auto itr=algorithms.begin();itr!=algorithms.end();itr++)
        {
            std::string temp=*itr.base();

            if(cost[temp][k]==bestTmp[k])
                bestCost_n[temp][n]++;

            if(pathLength[temp][k]==shortestTmp[k])
                shortestLength_n[temp][n]++;
        }
    }

   std::map<std::string,int> zero_n;

    for(size_t i=0;i<list_of_n.size();i++)
    {
        int n = list_of_n[i];
        for(auto itr = algorithms.begin();itr!=algorithms.end();itr++)
        {

            std::string temp = *itr.base();
            zero_n[temp]=0;
            int help = number_of_mazes_n[n]-fails_n[temp][n];
            if(help >0)
            {
                totalTime_n[temp][n] = totalTime_n[temp][n]/help;
                totalLength_n[temp][n] = totalLength_n[temp][n]/help;
                totalCost_n[temp][n] =totalCost_n[temp][n]/help;
                totalNode_n[temp][n] =totalNode_n[temp][n]/help;
            }
            else
            {
                totalTime_n[temp][n] = 0;
                totalLength_n[temp][n] = 0;
                totalCost_n[temp][n] =0;
                totalNode_n[temp][n] =0;
                zero_n[temp]++;
            }
        }

        totalTime_A_H1+=totalTime_n["A_H1"][n];
        totalTime_A_H2+=totalTime_n["A_H2"][n];
        totalTime_IDA_H1+=totalTime_n["IDA_H1"][n];
        totalTime_IDA_H2+=totalTime_n["IDA_H2"][n];

        totalNode_A_H1+=totalNode_n["A_H1"][n];
        totalNode_A_H2+=totalNode_n["A_H2"][n];
        totalNode_IDA_H1+=totalNode_n["IDA_H1"][n];
        totalNode_IDA_H2+=totalNode_n["IDA_H2"][n];

        totalCost_A_H1+=totalCost_n["A_H1"][n];
        totalCost_A_H2+=totalCost_n["A_H2"][n];
        totalCost_IDA_H1+=totalCost_n["IDA_H1"][n];
        totalCost_IDA_H2+=totalCost_n["IDA_H2"][n];

        totalLength_A_H1+=totalLength_n["A_H1"][n];
        totalLength_A_H2+=totalLength_n["A_H2"][n];
        totalLength_IDA_H1+=totalLength_n["IDA_H1"][n];
        totalLength_IDA_H2+=totalLength_n["IDA_H2"][n];

        totalFails_A_H1 += fails_n["A_H1"][n];
        totalFails_A_H2 += fails_n["A_H2"][n];

        totalFails_IDA_H1 += fails_n["IDA_H1"][n];
        totalFails_IDA_H2 += fails_n["IDA_H2"][n];

        totalBestCnt_A_H1 += bestCost_n["A_H1"][n];
        totalBestCnt_A_H2 += bestCost_n["A_H2"][n];

        totalBestCnt_IDA_H1 += bestCost_n["IDA_H1"][n];
        totalBestCnt_IDA_H2 += bestCost_n["IDA_H2"][n];

        totalShortestCnt_A_H1 += shortestLength_n["A_H1"][n];
        totalShortestCnt_A_H2 += shortestLength_n["A_H2"][n];

        totalShortestCnt_IDA_H1 += shortestLength_n["IDA_H1"][n];
        totalShortestCnt_IDA_H2 += shortestLength_n["IDA_H2"][n];
    }

    int helps = list_of_n.size()-zero_n["A_H1"];
    if(helps<1)
        helps = 1;

    totalTime_A_H1= totalTime_A_H1/helps;
    totalCost_A_H1 = totalCost_A_H1/helps;
    totalNode_A_H1 = totalNode_A_H1/helps;
    totalLength_A_H1 = totalLength_A_H1/helps;

    helps = list_of_n.size()-zero_n["A_H2"];
        if(helps<1)
            helps = 1;

    totalTime_A_H2=totalTime_A_H2/helps;
    totalCost_A_H2 = totalCost_A_H2/helps;
    totalNode_A_H2 = totalNode_A_H2/helps;
    totalLength_A_H2 = totalLength_A_H2/helps;

    helps = list_of_n.size()-zero_n["IDA_H1"];
        if(helps<1)
            helps = 1;

    totalTime_IDA_H1= totalTime_IDA_H1/helps;
    totalCost_IDA_H1 = totalCost_IDA_H1/helps;
    totalNode_IDA_H1 = totalNode_IDA_H1/helps;
    totalLength_IDA_H1 = totalLength_IDA_H1/helps;

    helps = list_of_n.size()-zero_n["IDA_H2"];
        if(helps<1)
            helps = 1;

    totalTime_IDA_H2=totalTime_IDA_H2/helps;
    totalCost_IDA_H2 = totalCost_IDA_H2/helps;
    totalNode_IDA_H2 = totalNode_IDA_H2/helps;
    totalLength_IDA_H2 = totalLength_IDA_H2/helps;

    totalTime_A = (totalTime_A_H1+totalTime_A_H2)/2;
    totalTime_IDA = (totalTime_IDA_H1+totalTime_IDA_H2)/2;

    totalCost_A = (totalCost_A_H1+totalCost_A_H2)/2;
    totalCost_IDA = (totalCost_IDA_H1+totalCost_IDA_H2)/2;

    totalNode_A = (totalNode_A_H1+totalNode_A_H2)/2;
    totalNode_IDA = (totalNode_IDA_H1+totalNode_IDA_H2)/2;

    totalLength_A = (totalLength_A_H1+totalLength_A_H2)/2;
    totalLength_IDA = (totalLength_IDA_H1+totalLength_IDA_H2)/2;

    totalFails_A = totalFails_A_H1+totalFails_A_H2;
    totalFails_IDA = totalFails_IDA_H1+totalFails_IDA_H2;

    totalBestCnt_A = (totalBestCnt_A_H1+totalBestCnt_A_H2);
    totalBestCnt_IDA = (totalBestCnt_IDA_H1+totalBestCnt_IDA_H2);

    totalShortestCnt_A = totalShortestCnt_A_H1+totalShortestCnt_A_H2;
    totalShortestCnt_IDA = totalShortestCnt_IDA_H1+totalShortestCnt_IDA_H2;


    std::ofstream outputFile;
    std::string outName = outputDir;

    if(outputDir[0]=='.')
        outputDir.erase(0,1);

    if(outputDir[0]=='\\')
        outputDir.erase(0,1);

    if(outputDir[outputDir.length()-1]=='\\')
        outputDir.erase(outputDir.length()-1,1);

    outName+=std::string("\\")+"SUMMARY_"+outputDir+".txt";

    outputFile.open(outName);
    if(!outputFile.is_open())
    {
        std::cout<<"Problem z utwrzoeniem pliku. Sprawdz ustawienia i katalog doceluwy\n";
        return -1;
    }

    if(show_in_cmd)
    {
        std::cout<<"===============================================================================\n";
        std::cout<<"PODSUMOWANIE OGOLNE\n";
        std::cout<<"t - czas, n - node'y, p - dl. sciezki, c - koszt, f - porazki\n";
        std::cout<<"bc - ile razy najmniejszy koszt, bp - ile razy najkrotsza sciezka\n";
        std::cout<<"-------------------------------------------------------------------------------\n";
        std::cout<<"Algorytm | "<<std::setw(6)<<"avg(t) | "<<std::setw(9)<<"avg(n) | "<<std::setw(3)<<"avg(p) | "<<std::setw(8)<<"avg(c) | "<<std::setw(8)<<"count(bp) | "<<std::setw(8)<<"count(bc) | "<<std::setw(8)<<"count(f)"<<std::endl;
        std::cout<<"-------------------------------------------------------------------------------\n";
        std::cout<<"alg_A      "<<std::setw(4)<<totalTime_A<<"ms"<<std::setw(8)<<totalNode_A<<std::setw(9)<<totalLength_A<<std::setw(9)<<totalCost_A<<std::setw(8)<<totalShortestCnt_A<<std::setw(13)<<totalBestCnt_A<<std::setw(11)<<totalFails_A<<std::endl;
        std::cout<<"alg_IDA    "<<std::setw(4)<<totalTime_IDA<<"ms"<<std::setw(8)<<totalNode_IDA<<std::setw(9)<<totalLength_IDA<<std::setw(9)<<totalCost_IDA<<std::setw(8)<<totalShortestCnt_IDA<<std::setw(13)<<totalBestCnt_IDA<<std::setw(11)<<totalFails_IDA<<std::endl;
        std::cout<<"-------------------------------------------------------------------------------\n";
        std::cout<<"alg_A_H1   "<<std::setw(4)<<totalTime_A_H1<<"ms"<<std::setw(8)<<totalNode_A_H1<<std::setw(9)<<totalLength_A_H1<<std::setw(9)<<totalCost_A_H1<<std::setw(8)<<totalShortestCnt_A_H1<<std::setw(13)<<totalBestCnt_A_H1<<std::setw(11)<<totalFails_A_H1<<std::endl;
        std::cout<<"alg_A_H2   "<<std::setw(4)<<totalTime_A_H2<<"ms"<<std::setw(8)<<totalNode_A_H2<<std::setw(9)<<totalLength_A_H2<<std::setw(9)<<totalCost_A_H2<<std::setw(8)<<totalShortestCnt_A_H2<<std::setw(13)<<totalBestCnt_A_H2<<std::setw(11)<<totalFails_A_H2<<std::endl;
        std::cout<<"alg_IDA_H1 "<<std::setw(4)<<totalTime_IDA_H1<<"ms"<<std::setw(8)<<totalNode_IDA_H1<<std::setw(9)<<totalLength_IDA_H1<<std::setw(9)<<totalCost_IDA_H1<<std::setw(8)<<totalShortestCnt_IDA_H1<<std::setw(13)<<totalBestCnt_IDA_H1<<std::setw(11)<<totalFails_IDA_H1<<std::endl;
        std::cout<<"alg_IDA_H2 "<<std::setw(4)<<totalTime_IDA_H2<<"ms"<<std::setw(8)<<totalNode_IDA_H2<<std::setw(9)<<totalLength_IDA_H2<<std::setw(9)<<totalCost_IDA_H2<<std::setw(8)<<totalShortestCnt_IDA_H2<<std::setw(13)<<totalBestCnt_IDA_H2<<std::setw(11)<<totalFails_IDA_H2<<std::endl;
        std::cout<<"===============================================================================\n";
    }

    outputFile<<"===============================================================================\n";
    outputFile<<"PODSUMOWANIE OGOLNE\n";
    outputFile<<"t - czas, n - node'y, p - dl. sciezki, c - koszt, f - porazki\n";
    outputFile<<"bc - ile razy najmniejszy koszt, bp - ile razy najkrotsza sciezka\n";
    outputFile<<"-------------------------------------------------------------------------------\n";
    outputFile<<"Algorytm | "<<std::setw(6)<<"avg(t) | "<<std::setw(9)<<"avg(n) | "<<std::setw(3)<<"avg(p) | "<<std::setw(8)<<"avg(c) | "<<std::setw(8)<<"count(bp) | "<<std::setw(8)<<"count(bc) | "<<std::setw(8)<<"count(f)"<<std::endl;
    outputFile<<"-------------------------------------------------------------------------------\n";
    outputFile<<"alg_A      "<<std::setw(4)<<totalTime_A<<"ms"<<std::setw(8)<<totalNode_A<<std::setw(9)<<totalLength_A<<std::setw(9)<<totalCost_A<<std::setw(8)<<totalShortestCnt_A<<std::setw(13)<<totalBestCnt_A<<std::setw(11)<<totalFails_A<<std::endl;
    outputFile<<"alg_IDA    "<<std::setw(4)<<totalTime_IDA<<"ms"<<std::setw(8)<<totalNode_IDA<<std::setw(9)<<totalLength_IDA<<std::setw(9)<<totalCost_IDA<<std::setw(8)<<totalShortestCnt_IDA<<std::setw(13)<<totalBestCnt_IDA<<std::setw(11)<<totalFails_IDA<<std::endl;
    outputFile<<"-------------------------------------------------------------------------------\n";
    outputFile<<"alg_A_H1   "<<std::setw(4)<<totalTime_A_H1<<"ms"<<std::setw(8)<<totalNode_A_H1<<std::setw(9)<<totalLength_A_H1<<std::setw(9)<<totalCost_A_H1<<std::setw(8)<<totalShortestCnt_A_H1<<std::setw(13)<<totalBestCnt_A_H1<<std::setw(11)<<totalFails_A_H1<<std::endl;
    outputFile<<"alg_A_H2   "<<std::setw(4)<<totalTime_A_H2<<"ms"<<std::setw(8)<<totalNode_A_H2<<std::setw(9)<<totalLength_A_H2<<std::setw(9)<<totalCost_A_H2<<std::setw(8)<<totalShortestCnt_A_H2<<std::setw(13)<<totalBestCnt_A_H2<<std::setw(11)<<totalFails_A_H2<<std::endl;
    outputFile<<"alg_IDA_H1 "<<std::setw(4)<<totalTime_IDA_H1<<"ms"<<std::setw(8)<<totalNode_IDA_H1<<std::setw(9)<<totalLength_IDA_H1<<std::setw(9)<<totalCost_IDA_H1<<std::setw(8)<<totalShortestCnt_IDA_H1<<std::setw(13)<<totalBestCnt_IDA_H1<<std::setw(11)<<totalFails_IDA_H1<<std::endl;
    outputFile<<"alg_IDA_H2 "<<std::setw(4)<<totalTime_IDA_H2<<"ms"<<std::setw(8)<<totalNode_IDA_H2<<std::setw(9)<<totalLength_IDA_H2<<std::setw(9)<<totalCost_IDA_H2<<std::setw(8)<<totalShortestCnt_IDA_H2<<std::setw(13)<<totalBestCnt_IDA_H2<<std::setw(11)<<totalFails_IDA_H2<<std::endl;
    outputFile<<"===============================================================================\n";

    for(size_t i =0;i<list_of_n.size();i++)
    {
        int n = list_of_n[i];
        int total_A = (totalTime_n["A_H1"][n]+totalTime_n["A_H2"][n])/2;
        int total_IDA = (totalTime_n["IDA_H1"][n]+totalTime_n["IDA_H2"][n])/2;

        int total_node_A = (totalNode_n["A_H1"][n]+totalNode_n["A_H2"][n])/2;
        int total_node_IDA = (totalNode_n["IDA_H1"][n]+totalNode_n["IDA_H2"][n])/2;

        int total_length_A = (totalLength_n["A_H1"][n]+totalLength_n["A_H2"][n])/2;
        int total_length_IDA = (totalLength_n["IDA_H1"][n]+totalLength_n["IDA_H2"][n])/2;

        int total_cost_A = (totalCost_n["A_H1"][n]+totalCost_n["A_H2"][n])/2;
        int total_cost_IDA = (totalCost_n["IDA_H1"][n]+totalCost_n["IDA_H2"][n])/2;

        int total_shortest_A = (shortestLength_n["A_H1"][n]+shortestLength_n["A_H2"][n]);
        int total_shortest_IDA = (shortestLength_n["IDA_H1"][n]+shortestLength_n["IDA_H2"][n]);

        int total_best_A = (bestCost_n["A_H1"][n]+bestCost_n["A_H2"][n]);
        int total_best_IDA = (bestCost_n["IDA_H1"][n]+bestCost_n["IDA_H2"][n]);

        int total_fails_A = (fails_n["A_H1"][n]+fails_n["A_H2"][n]);
        int total_fails_IDA = (fails_n["IDA_H1"][n]+fails_n["IDA_H2"][n]);

        outputFile<<"\n===============================================================================\n";
        outputFile<<"PODSUMOWANIE dla labiryntu n="<<n<<"\n";
        outputFile<<"t - czas, n - node'y, p - dl. sciezki, c - koszt, f - porazki\n";
        outputFile<<"bc - ile razy najmniejszy koszt, bp - ile razy najkrotsza sciezka\n";
        outputFile<<"-------------------------------------------------------------------------------\n";
        outputFile<<"Algorytm | "<<std::setw(4)<<"avg(t) | "<<std::setw(9)<<"avg(n) | "<<std::setw(3)<<"avg(p) | "<<std::setw(8)<<"avg(c) | "<<std::setw(8)<<"count(bp) | "<<std::setw(8)<<"count(bc) | "<<std::setw(8)<<"count(f)"<<std::endl;
        outputFile<<"-------------------------------------------------------------------------------\n";
        outputFile<<"alg_A      "<<std::setw(4)<<total_A<<"ms"<<std::setw(8)<<total_node_A<<std::setw(9)<<total_length_A<<std::setw(9)<<total_cost_A<<std::setw(8)<<total_shortest_A<<std::setw(13)<<total_best_A<<std::setw(11)<<total_fails_A<<std::endl;
        outputFile<<"alg_IDA    "<<std::setw(4)<<total_IDA<<"ms"<<std::setw(8)<<total_node_IDA<<std::setw(9)<<total_length_IDA<<std::setw(9)<<total_cost_IDA<<std::setw(8)<<total_shortest_IDA<<std::setw(13)<<total_best_IDA<<std::setw(11)<<total_fails_IDA<<std::endl;
        outputFile<<"-------------------------------------------------------------------------------\n";
        outputFile<<"alg_A_H1   "<<std::setw(4)<<totalTime_n["A_H1"][n]<<"ms"<<std::setw(8)<<totalNode_n["A_H1"][n]<<std::setw(9)<<totalLength_n["A_H1"][n]<<std::setw(9)<<totalCost_n["A_H1"][n]<<std::setw(8)<<shortestLength_n["A_H1"][n]<<std::setw(13)<<bestCost_n["A_H1"][n]<<std::setw(11)<<fails_n["A_H1"][n]<<std::endl;
        outputFile<<"alg_A_H2   "<<std::setw(4)<<totalTime_n["A_H2"][n]<<"ms"<<std::setw(8)<<totalNode_n["A_H2"][n]<<std::setw(9)<<totalLength_n["A_H2"][n]<<std::setw(9)<<totalCost_n["A_H2"][n]<<std::setw(8)<<shortestLength_n["A_H2"][n]<<std::setw(13)<<bestCost_n["A_H2"][n]<<std::setw(11)<<fails_n["A_H2"][n]<<std::endl;
        outputFile<<"alg_IDA_H1 "<<std::setw(4)<<totalTime_n["IDA_H1"][n]<<"ms"<<std::setw(8)<<totalNode_n["IDA_H1"][n]<<std::setw(9)<<totalLength_n["IDA_H1"][n]<<std::setw(9)<<totalCost_n["IDA_H1"][n]<<std::setw(8)<<shortestLength_n["IDA_H1"][n]<<std::setw(13)<<bestCost_n["IDA_H1"][n]<<std::setw(11)<<fails_n["IDA_H1"][n]<<std::endl;
        outputFile<<"alg_IDA_H2 "<<std::setw(4)<<totalTime_n["IDA_H2"][n]<<"ms"<<std::setw(8)<<totalNode_n["IDA_H2"][n]<<std::setw(9)<<totalLength_n["IDA_H2"][n]<<std::setw(9)<<totalCost_n["IDA_H2"][n]<<std::setw(8)<<shortestLength_n["IDA_H2"][n]<<std::setw(13)<<bestCost_n["IDA_H2"][n]<<std::setw(11)<<fails_n["IDA_H2"][n]<<std::endl;
        outputFile<<"===============================================================================\n";
    }
    outputFile.close();

    return 0;
}
