#include "functions.h"
#include "algorithms.h"
#include <algorithm>
#include <chrono>
#include <iomanip>

#include "summary.h"

int main(int argc, char *argv[])
{
    std::vector<Maze> mazes; // lista labiryntów
    std::map<std::string,char> settings; // mapa ustawień
    std::string outputDir = ".\\output\\"; // domyślna ścieżka wyjścia
    std::string inputDir  = ".\\input\\"; // domyślna ścieżwa wejścia
    int numTests = 1; // domyślna liczba uruchomień algorytmów na labirynt

    Summary summary(&mazes);

    /*
     * listy, do których można będzie zapisać wyniki częściowe
     */
    std::vector<long long int> time_A_H1,time_A_H2,time_IDA_H1,time_IDA_H2,
            numNodes_A_H1, numNodes_A_H2,numNodes_IDA_H1, numNodes_IDA_H2,
            lengthPath_A_H1,lengthPath_A_H2,lengthPath_IDA_H1,lengthPath_IDA_H2,
            cost_A_H1,cost_A_H2, cost_IDA_H1, cost_IDA_H2;
    std::vector<std::string> paths_A_H1,paths_A_H2,paths_IDA_H1,paths_IDA_H2;



    /*
     * ładowanie argumentów CMD
     */
    if(argc<2)
        std::cout<<"Brak parametrow. Zaladowane zostana ustawienia domyslne.\n";
    else
        load_arguments(argc,argv,inputDir,outputDir,numTests);

    summary.set_repeats(numTests);

    /*
     * ładowanie ustawień z pliku settings.txt
     */
    load_settings(settings);

    // ładowanie labiryntów
    mazes = load_files(inputDir,settings);

    if(mazes.size()<1)
    {
        std::cout<<"Bledna nazwa folderu, niewlasciwa sciezka lub folder pusty";
        return -1;
    }

    generate_states(mazes); // generowanie stanów
    generate_actions(mazes); // generowanie akcji

    std::ofstream outputFile;
    std::string outName;

    int result_g = 0, numNodes =0;
    std::vector<AbstractState*> path;

    for(size_t i =0;i<mazes.size();i++)
    {
        long long int average_time =0;
        int average_nodes=0;
        int average_length=0;
        int average_costs=0;


        int idx_H1=-1;
        int idx_H2=-1;

        State *startState = mazes[i].getStartState();
        State *endState = mazes[i].getEndState();

        if(endState==nullptr || startState==nullptr)
        {
            std::cout<<"Brak punktu startowego i/lub koncowego";
            std::cout<<"\nLabirynt zostanie pominiety:"<<mazes[i].get_file_name()<<"\n";
            continue;
        }

        if(settings["heurestic_1"]=='t' || settings["heurestic_1"]=='T')
        {
            idx_H1 = mazes[i].generate_H(H1);
            outName = outputDir+"output_STATES_H1_"+mazes[i].get_file_name();
            outputFile.open(outName);

            if(!outputFile.is_open())
            {
                if(opendir(outputDir.c_str())==NULL)
                    mkdir(outputDir.c_str());
                outputFile.open(outName);
            }

            if(outputFile.is_open())
            {
                outputFile<<"Wygenerowane stany(heurestyki)|\t Akcje(koszty)\n\n";
                for(size_t j = 0;j<mazes[i].getStates()->size();j++)
                {
                    State s = mazes[i].getStates()->at(j);

                    outputFile<<s.get_name()<<"("<<s.get_h(idx_H1)<<")"<<"\t";

                    if(s.is_end())
                        outputFile<<"KONIEC";
                    if(s.is_start())
                        outputFile<<"START";

                    outputFile<<"\t\t\t [";
                    std::map<AbstractState*,int> *m = s.get_nexts();

                    for(auto itr = m->begin();itr!=m->end();itr++)
                    {
                        if(itr!=m->begin())
                            outputFile<<",";
                        outputFile<<itr->first->get_name()<<"("<<itr->second<<")";
                    }
                    outputFile<<"]"<<std::endl;
                }
                outputFile.close();
            }
            else
            {
                std::cout<<"Nie mozna utworzyc pliku wyjsciowego.\n";
            }
        }

        if(settings["heurestic_2"]=='t' || settings["heurestic_2"]=='T')
        {
            idx_H2 = mazes[i].generate_H(H2);
            outName = outputDir+"output_STATES_H2_"+mazes[i].get_file_name();
            outputFile.open(outName);

            if(!outputFile.is_open())
            {
                if(opendir(outputDir.c_str())==NULL)
                    mkdir(outputDir.c_str());
                outputFile.open(outName);
            }

            if(outputFile.is_open())
            {
                outputFile<<"Wygenerowane stany(heurestyki)|\t Akcje(koszty)\n\n";
                for(size_t j = 0;j<mazes[i].getStates()->size();j++)
                {
                    State s = mazes[i].getStates()->at(j);
                    outputFile<<s.get_name()<<"("<<s.get_h(idx_H2)<<")"<<"\t";
                    if(s.is_end())
                        outputFile<<"KONIEC";
                    if(s.is_start())
                        outputFile<<"START";
                    outputFile<<"\t\t\t [";
                    std::map<AbstractState*,int> *m = s.get_nexts();
                    for(auto itr = m->begin();itr!=m->end();itr++)
                    {
                        if(itr!=m->begin())
                            outputFile<<",";
                        outputFile<<itr->first->get_name()<<"("<<itr->second<<")";
                    }
                    outputFile<<"]"<<std::endl;
                }
                outputFile.close();
            }
            else
            {
                std::cout<<"Nie mozna utworzyc pliku wyjsciowego.\n";
            }

        }

        outName = outputDir+"output_RESULTS_"+mazes[i].get_file_name();
        outputFile.open(outName);

        if(!outputFile.is_open())
        {
            if(opendir(outputDir.c_str())==NULL)
                mkdir(outputDir.c_str());
            outputFile.open(outName);
        }

        if(!outputFile.is_open())
        {
            std::cout<<"\nNie mozna bylo utworzyc pliku wyjsciowego\nWyniki nie zostana zapisane\n";
        }

        if(settings["out_cmd"]=='t' || settings["out_cmd"]=='T')
            std::cout<<summary.get_results_head(mazes[i].get_file_name());


        if(outputFile.is_open())
            outputFile<<summary.get_results_head(mazes[i].get_file_name());

        int isFound = -1; //nie znaleziono ścieżki

        if((settings["alg_A"]=='t' ||settings["alg_A"]=='T') && (settings["heurestic_1"]=='t' || settings["heurestic_1"]=='T'))
        {
            time_A_H1.push_back(0);
            numNodes_A_H1.push_back(0);
            lengthPath_A_H1.push_back(0);
            cost_A_H1.push_back(0);
/*
            for(int xz=0;xz<numTests;xz++)
            {
                path.clear();

                auto start = std::chrono::high_resolution_clock::now();
                A_no_nodes(startState,idx_H1,&result_g,&path);
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration_A_H1= std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);

                time_A_H1[i]+=duration_A_H1.count();

                result_g =0;
                path.clear();

                isFound = A(startState,idx_H1,&result_g,&path,&numNodes);

                numNodes_A_H1[i]+=numNodes;
                lengthPath_A_H1[i]+=path.size();
                cost_A_H1[i]+=result_g;

                result_g =0;
                numNodes = 0;
            }

            std::string temp;
            for(size_t asd=0;asd<path.size();asd++)
            {
                temp+=path[asd]->get_name();
                if(asd!=path.size()-1)
                    temp+="-->";
            }

            paths_A_H1.push_back(temp);

            if(isFound==0)
            {
                average_time = time_A_H1[i]/numTests;
                average_nodes = numNodes_A_H1[i]/numTests;
                average_length = lengthPath_A_H1[i]/numTests;
                average_costs = cost_A_H1[i]/numTests;
            }
            else
            {
                average_time = 0;
                average_nodes = 0;
                average_costs = 0;
                average_length = 0;
            }*/

            summary.run(A,"A_H1",i,idx_H1,true);

            if(outputFile.is_open())
                outputFile<<"A_H1"<<std::setw(11)<<average_time<<"ms"<<std::setw(14)<<average_nodes<<std::setw(9)<<average_length<<std::setw(16)<<average_costs<<std::endl;

            if(settings["out_cmd"]=='t' || settings["out_cmd"]=='T')
            {
                //std::cout<<"A_H1"<<std::setw(11)<<average_time<<"ms"<<std::setw(14)<<average_nodes<<std::setw(9)<<average_length<<std::setw(16)<<average_costs<<std::endl;
                std::cout<<"A_H1"<<std::setw(11)<<summary.get_time("A_H1",i)<<"ms"<<std::setw(14)<<summary.get_nodes("A_H1",i)<<std::setw(9)<<summary.get_pathLenght("A_H1",i)<<std::setw(16)<<summary.get_cost("A_H1",i)<<std::endl;
            }

            path.clear();
        }

        if((settings["alg_A"]=='t' ||settings["alg_A"]=='T') && (settings["heurestic_2"]=='t' || settings["heurestic_2"]=='T'))
        {
            time_A_H2.push_back(0);
            numNodes_A_H2.push_back(0);
            lengthPath_A_H2.push_back(0);
            cost_A_H2.push_back(0);

            for(int xz=0;xz<numTests;xz++)
            {
                path.clear();

                auto start = std::chrono::high_resolution_clock::now();
                A_no_nodes(startState,idx_H2,&result_g,&path);
                auto stop = std::chrono::high_resolution_clock::now();
                //std::chrono::duration<double, std::milli> duration_A_H2=(stop-start);
                auto duration_A_H2= std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);

                time_A_H2[i]+=duration_A_H2.count();

                result_g =0;
                path.clear();

                isFound = A(startState,idx_H2,&result_g,&path,&numNodes);

                numNodes_A_H2[i]+=numNodes;
                lengthPath_A_H2[i]+=path.size();
                cost_A_H2[i]+=result_g;

                result_g =0;
                numNodes = 0;
            }

            std::string temp;
            for(size_t asd=0;asd<path.size();asd++)
            {
                temp+=path[asd]->get_name();
                if(asd!=path.size()-1)
                    temp+="-->";
            }

            paths_A_H2.push_back(temp);

            if(isFound==0)
            {
                average_time = time_A_H2[i]/numTests;
                average_nodes = numNodes_A_H2[i]/numTests;
                average_length = lengthPath_A_H2[i]/numTests;
                average_costs = cost_A_H2[i]/numTests;
            }
            else
            {
                average_time = 0;
                average_nodes = 0;
                average_costs = 0;
                average_length = 0;
            }

            if(outputFile.is_open())
                outputFile<<"A_H2"<<std::setw(11)<<average_time<<"ms"<<std::setw(14)<<average_nodes<<std::setw(9)<<average_length<<std::setw(16)<<average_costs<<std::endl;

            if(settings["out_cmd"]=='t' || settings["out_cmd"]=='T')
                std::cout<<"A_H2"<<std::setw(11)<<average_time<<"ms"<<std::setw(14)<<average_nodes<<std::setw(9)<<average_length<<std::setw(16)<<average_costs<<std::endl;

            path.clear();
        }

        if((settings["alg_IDA"]=='t' ||settings["alg_IDA"]=='T') && (settings["heurestic_1"]=='t' || settings["heurestic_1"]=='T'))
        {
            time_IDA_H1.push_back(0);
            numNodes_IDA_H1.push_back(0);
            lengthPath_IDA_H1.push_back(0);
            cost_IDA_H1.push_back(0);
            for(int xz=0;xz<numTests;xz++)
            {
                path.clear();

                auto start = std::chrono::high_resolution_clock::now();
                IDA_no_nodes(startState,idx_H1,&result_g,&path);
                auto stop = std::chrono::high_resolution_clock::now();
                //std::chrono::duration<double, std::milli> duration_IDA_H1=(stop-start);
                auto duration_IDA_H1= std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);

                time_IDA_H1[i]+=duration_IDA_H1.count();

                result_g =0;
                path.clear();

                IDA(startState,idx_H1,&result_g,&path,&numNodes);

                numNodes_IDA_H1[i]+=numNodes;
                lengthPath_IDA_H1[i]+=path.size();
                cost_IDA_H1[i]+=result_g;

                result_g =0;
                numNodes = 0;
            }

            std::string temp;
            for(size_t asd=0;asd<path.size();asd++)
            {
                temp+=path[asd]->get_name();
                if(asd!=path.size()-1)
                    temp+="-->";
            }

            paths_IDA_H1.push_back(temp);

            if(isFound==0)
            {
                average_time = time_IDA_H1[i]/numTests;
                average_nodes = numNodes_IDA_H1[i]/numTests;
                average_length = lengthPath_IDA_H1[i]/numTests;
                average_costs = cost_IDA_H1[i]/numTests;
            }
            else
            {
                average_time = 0;
                average_nodes = 0;
                average_costs = 0;
                average_length = 0;
            }

            if(outputFile.is_open())
                outputFile<<"IDA_H1"<<std::setw(9)<<average_time<<"ms"<<std::setw(14)<<average_nodes<<std::setw(9)<<average_length<<std::setw(16)<<average_costs<<std::endl;

            if(settings["out_cmd"]=='t' || settings["out_cmd"]=='T')
                std::cout<<"IDA_H1"<<std::setw(9)<<average_time<<"ms"<<std::setw(14)<<average_nodes<<std::setw(9)<<average_length<<std::setw(16)<<average_costs<<std::endl;

            path.clear();
        }

        if((settings["alg_IDA"]=='t' ||settings["alg_IDA"]=='T') && (settings["heurestic_2"]=='t' || settings["heurestic_2"]=='T'))
        {

            time_IDA_H2.push_back(0);
            numNodes_IDA_H2.push_back(0);
            lengthPath_IDA_H2.push_back(0);
            cost_IDA_H2.push_back(0);

            for(int xz=0;xz<numTests;xz++)
            {
                path.clear();

                auto start = std::chrono::high_resolution_clock::now();
                IDA_no_nodes(startState,idx_H2,&result_g,&path);
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration_IDA_H2= std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);

                time_IDA_H2[i]+=duration_IDA_H2.count();

                result_g =0;
                path.clear();

                isFound = IDA(startState,idx_H2,&result_g,&path,&numNodes);

                numNodes_IDA_H2[i]+=numNodes;
                lengthPath_IDA_H2[i]+=path.size();
                cost_IDA_H2[i]+=result_g;

                result_g =0;
                numNodes = 0;
            }

            std::string temp;
            for(size_t asd=0;asd<path.size();asd++)
            {
                temp+=path[asd]->get_name();
                if(asd!=path.size()-1)
                    temp+="-->";
            }

            paths_IDA_H2.push_back(temp);

            if(isFound==0)
            {
                average_time = time_IDA_H2[i]/numTests;
                average_nodes = numNodes_IDA_H2[i]/numTests;
                average_length = lengthPath_IDA_H2[i]/numTests;
                average_costs = cost_IDA_H2[i]/numTests;
            }
            else
            {
                average_time = 0;
                average_nodes = 0;
                average_costs = 0;
                average_length = 0;
            }

            if(outputFile.is_open())
                outputFile<<"IDA_H2"<<std::setw(9)<<average_time<<"ms"<<std::setw(14)<<average_nodes<<std::setw(9)<<average_length<<std::setw(16)<<average_costs<<std::endl;

            if(settings["out_cmd"]=='t' || settings["out_cmd"]=='T')
                std::cout<<"IDA_H2"<<std::setw(9)<<average_time<<"ms"<<std::setw(14)<<average_nodes<<std::setw(9)<<average_length<<std::setw(16)<<average_costs<<std::endl;

            path.clear();
        }

        if(outputFile.is_open())
            outputFile<<"======================================================================\n";

        if(settings["out_cmd"]=='t' || settings["out_cmd"]=='T')
            std::cout<<"======================================================================\n";

        if(outputFile.is_open())
        {
            if(settings["show_maze_file"]=='t' || settings["show_maze_file"]=='T')
                outputFile<<mazes[i].getArea()<<std::endl;
        }

        if(settings["out_cmd"]=='t' || settings["out_cmd"]=='T')
        {
            if(settings["show_maze_cmd"]=='t' || settings["show_maze_cmd"]=='T')
                std::cout<<mazes[i].getArea()<<std::endl;
        }

        outputFile.close();

        outName = outputDir+"output_PATHS_"+mazes[i].get_file_name();
        outputFile.open(outName);

        if(!outputFile.is_open())
        {
            if(opendir(outputDir.c_str())==NULL)
                mkdir(outputDir.c_str());
            outputFile.open(outName);
        }

        if(outputFile.is_open())
        {
            if((settings["alg_A"]=='t' ||settings["alg_A"]=='T') && (settings["heurestic_1"]=='t' || settings["heurestic_1"]=='T'))
                outputFile<<summary.get_paths_head(summary.get_path_string("A_H1",i),"A_H1");

            if((settings["alg_A"]=='t' ||settings["alg_A"]=='T') && (settings["heurestic_2"]=='t' || settings["heurestic_2"]=='T'))
                outputFile<<summary.get_paths_head(summary.get_path_string("A_H2",i),"A_H2");

            if((settings["alg_IDA"]=='t' ||settings["alg_IDA"]=='T') && (settings["heurestic_1"]=='t' || settings["heurestic_1"]=='T'))
                outputFile<<summary.get_paths_head(summary.get_path_string("IDA_H1",i),"IDA_H1");

            if((settings["alg_IDA"]=='t' ||settings["alg_IDA"]=='T') && (settings["heurestic_2"]=='t' || settings["heurestic_2"]=='T'))
                outputFile<<summary.get_paths_head(summary.get_path_string("IDA_H2",i),"IDA_H2");
        }

        if(settings["show_path_cmd"]=='t' || settings["show_path_cmd"]=='T')
        {
            if((settings["alg_A"]=='t' ||settings["alg_A"]=='T') && (settings["heurestic_1"]=='t' || settings["heurestic_1"]=='T'))
                std::cout<<summary.get_paths_head(paths_A_H1[i],"A_H1");

            if((settings["alg_A"]=='t' ||settings["alg_A"]=='T') && (settings["heurestic_2"]=='t' || settings["heurestic_2"]=='T'))
                std::cout<<summary.get_paths_head(paths_A_H2[i],"A_H2");

            if((settings["alg_IDA"]=='t' ||settings["alg_IDA"]=='T') && (settings["heurestic_1"]=='t' || settings["heurestic_1"]=='T'))
                std::cout<<summary.get_paths_head(paths_IDA_H1[i],"IDA_H1");

            if((settings["alg_IDA"]=='t' ||settings["alg_IDA"]=='T') && (settings["heurestic_2"]=='t' || settings["heurestic_2"]=='T'))
                std::cout<<summary.get_paths_head(paths_IDA_H2[i],"IDA_H2");
        }

        outputFile.close();
    }
    return 0;
}
