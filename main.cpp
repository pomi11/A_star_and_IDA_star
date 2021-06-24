#include "algorithms.h"
#include "functions.h"
#include <algorithm>
#include <chrono>
#include <iomanip>

#include "summary.h"

long long int depth;

int main(int argc, char *argv[])
{
    std::vector<Maze> mazes; // lista labiryntów
    std::map<std::string,char> settings; // mapa ustawień
    std::string outputDir = ".\\output\\"; // domyślna ścieżka wyjścia
    std::string inputDir  = ".\\input\\"; // domyślna ścieżwa wejścia
    int numTests = 1; // domyślna liczba uruchomień algorytmów na labirynt
    depth = 999999999999999999; //domyślna wartość maksymalnego pogłębienia algorytmu IDA;

    Summary summary(&mazes,outputDir);

    /*
     * ładowanie argumentów CMD
     */
    if(argc<2)
        std::cout<<"Brak parametrow. Zaladowane zostana ustawienia domyslne.\n";
    else
        load_arguments(argc,argv,inputDir,outputDir,numTests, depth);

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

    std::vector<AbstractState*> path;

    for(size_t i =0;i<mazes.size();i++)
    {
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

        if((settings["alg_A"]=='t' ||settings["alg_A"]=='T') && (settings["heurestic_1"]=='t' || settings["heurestic_1"]=='T'))
        {
            summary.run(A,A_no_nodes,"A_H1",i,idx_H1,false);

            if(outputFile.is_open())
                outputFile<<"A_H1"<<std::setw(11)<<summary.get_time("A_H1",i)<<"ms"<<std::setw(14)<<summary.get_nodes("A_H1",i)<<std::setw(9)<<summary.get_pathLenght("A_H1",i)<<std::setw(16)<<summary.get_cost("A_H1",i)<<std::endl;

            if(settings["out_cmd"]=='t' || settings["out_cmd"]=='T')
                std::cout<<"A_H1"<<std::setw(11)<<summary.get_time("A_H1",i)<<"ms"<<std::setw(14)<<summary.get_nodes("A_H1",i)<<std::setw(9)<<summary.get_pathLenght("A_H1",i)<<std::setw(16)<<summary.get_cost("A_H1",i)<<std::endl;
        }

        if((settings["alg_A"]=='t' ||settings["alg_A"]=='T') && (settings["heurestic_2"]=='t' || settings["heurestic_2"]=='T'))
        {
            summary.run(A,A_no_nodes,"A_H2",i,idx_H2,false);

            if(outputFile.is_open())
                outputFile<<"A_H2"<<std::setw(11)<<summary.get_time("A_H2",i)<<"ms"<<std::setw(14)<<summary.get_nodes("A_H2",i)<<std::setw(9)<<summary.get_pathLenght("A_H2",i)<<std::setw(16)<<summary.get_cost("A_H2",i)<<std::endl;

            if(settings["out_cmd"]=='t' || settings["out_cmd"]=='T')
                std::cout<<"A_H2"<<std::setw(11)<<summary.get_time("A_H2",i)<<"ms"<<std::setw(14)<<summary.get_nodes("A_H2",i)<<std::setw(9)<<summary.get_pathLenght("A_H2",i)<<std::setw(16)<<summary.get_cost("A_H2",i)<<std::endl;
        }

        if((settings["alg_IDA"]=='t' ||settings["alg_IDA"]=='T') && (settings["heurestic_1"]=='t' || settings["heurestic_1"]=='T'))
        {
            summary.run(IDA,IDA_no_nodes,"IDA_H1",i,idx_H1,false);

            if(outputFile.is_open())
                outputFile<<"IDA_H1"<<std::setw(9)<<summary.get_time("IDA_H1",i)<<"ms"<<std::setw(14)<<summary.get_nodes("IDA_H1",i)<<std::setw(9)<<summary.get_pathLenght("IDA_H1",i)<<std::setw(16)<<summary.get_cost("IDA_H1",i)<<std::endl;

            if(settings["out_cmd"]=='t' || settings["out_cmd"]=='T')
                std::cout<<"IDA_H1"<<std::setw(9)<<summary.get_time("IDA_H1",i)<<"ms"<<std::setw(14)<<summary.get_nodes("IDA_H1",i)<<std::setw(9)<<summary.get_pathLenght("IDA_H1",i)<<std::setw(16)<<summary.get_cost("IDA_H1",i)<<std::endl;
        }

        if((settings["alg_IDA"]=='t' ||settings["alg_IDA"]=='T') && (settings["heurestic_2"]=='t' || settings["heurestic_2"]=='T'))
        {
            summary.run(IDA,IDA_no_nodes,"IDA_H2",i,idx_H2,false);

            if(outputFile.is_open())
                outputFile<<"IDA_H2"<<std::setw(9)<<summary.get_time("IDA_H2",i)<<"ms"<<std::setw(14)<<summary.get_nodes("IDA_H2",i)<<std::setw(9)<<summary.get_pathLenght("IDA_H2",i)<<std::setw(16)<<summary.get_cost("IDA_H2",i)<<std::endl;

            if(settings["out_cmd"]=='t' || settings["out_cmd"]=='T')
                std::cout<<"IDA_H2"<<std::setw(9)<<summary.get_time("IDA_H2",i)<<"ms"<<std::setw(14)<<summary.get_nodes("IDA_H2",i)<<std::setw(9)<<summary.get_pathLenght("IDA_H2",i)<<std::setw(16)<<summary.get_cost("IDA_H2",i)<<std::endl;
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
                outputFile<<summary.get_paths_head("A_H1",i);

            if((settings["alg_A"]=='t' ||settings["alg_A"]=='T') && (settings["heurestic_2"]=='t' || settings["heurestic_2"]=='T'))
                outputFile<<summary.get_paths_head("A_H2",i);

            if((settings["alg_IDA"]=='t' ||settings["alg_IDA"]=='T') && (settings["heurestic_1"]=='t' || settings["heurestic_1"]=='T'))
                outputFile<<summary.get_paths_head("IDA_H1",i);

            if((settings["alg_IDA"]=='t' ||settings["alg_IDA"]=='T') && (settings["heurestic_2"]=='t' || settings["heurestic_2"]=='T'))
                outputFile<<summary.get_paths_head("IDA_H2",i);
        }

        if(settings["show_path_cmd"]=='t' || settings["show_path_cmd"]=='T')
        {
            if((settings["alg_A"]=='t' ||settings["alg_A"]=='T') && (settings["heurestic_1"]=='t' || settings["heurestic_1"]=='T'))
                outputFile<<summary.get_paths_head("A_H1",i);

            if((settings["alg_A"]=='t' ||settings["alg_A"]=='T') && (settings["heurestic_2"]=='t' || settings["heurestic_2"]=='T'))
                outputFile<<summary.get_paths_head("A_H2",i);

            if((settings["alg_IDA"]=='t' ||settings["alg_IDA"]=='T') && (settings["heurestic_1"]=='t' || settings["heurestic_1"]=='T'))
                outputFile<<summary.get_paths_head("IDA_H1",i);

            if((settings["alg_IDA"]=='t' ||settings["alg_IDA"]=='T') && (settings["heurestic_2"]=='t' || settings["heurestic_2"]=='T'))
                outputFile<<summary.get_paths_head("IDA_H2",i);
        }

        outputFile.close();
    }
    if(settings["show_path_cmd"]=='t' || settings["show_path_cmd"]=='T')
        summary.save_summary(true);
    else
        summary.save_summary();
    return 0;
}
