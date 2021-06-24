#include "functions.h"

std::vector<Maze> load_files(const std::string &inputDir, std::map<std::string,char> &settings)
{
    std::vector<Maze> mazes;
    std::vector<std::string> files;
    
    DIR *dir;
    struct dirent *directory;
    int filesCount = 0;
    
    /*
     * odczytujemy pliki i zapisujemy ich nazwy
     */
    if((dir= opendir(inputDir.c_str()))!=NULL)
    {
        while((directory = readdir(dir))!=NULL)
        {
            std::string fileName = directory->d_name;
            if(fileName=="."||fileName=="..")
                continue;
            
            filesCount++;
            files.push_back(fileName);
        }
    }
    
    for(int i=0;i<filesCount;i++)
    {
        std::fstream fs;
        std::string inputFile=inputDir+files[i];
        fs.open(inputFile);
        std::string result,result2;
        
        while(!fs.eof())
        {
            std::getline(fs,result); //pobieramy linię pliku
            result+='\n'; //dodajemy "enter"
            result2+=result; //łączymy wszystkie linie
        }
        
        result2[result2.length()-1]='\0'; // usuwamy "enter" z ostatniej linii

        Maze rs = Maze(result2);

        /*
         * uzupełniamy obiekt typu Maze o odpowiednie dane
         */
        rs.set_file_name(files[i]); //nazwa pliku
        rs.setEndChar(settings["endChar"]); // znak końca
        rs.setStartChar(settings["startChar"]); //znak początku
        rs.setWallChar(settings["wallChar"]); //znak ściany

        char cost = settings["defaultCost"];
        std::string tmpCost;
        tmpCost=cost;
        int d = atoi(tmpCost.c_str());
        bool isNumber = is_number(tmpCost);
        if(isNumber)
            rs.setDefCost(d); //domyślny koszt pojedynczej akcji

        cost = settings["endCost"];
        tmpCost=cost;
        d = atoi(tmpCost.c_str());
        isNumber = is_number(tmpCost);
        if(isNumber)
            rs.setEndCost(d); //domyślny koszt akcji do stanu końcowego

        cost = settings["startCost"];
        tmpCost=cost;
        d = atoi(tmpCost.c_str());
        isNumber = is_number(tmpCost);
        if(isNumber)
            rs.setStartCost(d); //domyślny koszt akcji do stanu początkowego

        mazes.push_back(rs);
        fs.close();
    }
    closedir(dir);
    return mazes;
}

void generate_states(std::vector<Maze> &mazes)
{
    for(size_t i=0;i<mazes.size();i++)
    {
        std::vector<State> states;
        std::string row;
        
        row ="A"; //litera pierwszego wiersza
        int k = 1; // zmienna pomocnicza licząca numer aktualnej kolumny
        int r = 0; // zmienna pomocnicza do określania, czy dodać literę
        int n = 1; // zmienna licząca wielkość labiryntu
        
        for(size_t j=0;j<mazes[i].getArea().length();j++)
        {
            char square = mazes[i].getArea()[j];
            
            if(square=='\n') // jeśli natrafiono na "enter" zaktualizuj literę wiersza
            {
                n++;

                if(row[r]=='Z') // jeśli wiersz Z, trzeba dodać literę lub poprzednią zaktualizować
                {
                    
                    /*
                     * jeśli jest więcej niż jedna litera
                     * sprawdzić trzeba poprzednią literę, czy
                     * nie jest "Z"
                     * jeśli wszystkie litery wiersza to "Z"
                     * zamieniamy na "A" i dodajemy nową literę
                     */
                    if((r-1)>-1)
                        r = r-1;
                    else
                    {
                        for(size_t zz=0;zz<row.length();zz++)
                        {
                            row[zz]='A';
                        }

                        row +='A';
                        r = row.length()-1;
                        k=1;
                        continue;
                    }
                }
                
                row[r]++;
                k =1;
                continue;
            }
            
            if(square=='\0')
                break;
            
            char startChar = mazes[i].getStartChar();
            char endChar = mazes[i].getEndChar();
            //char wallChar = mazes[i].getWallChar();
            
            std::string adress=row+std::to_string(k);
            std::string s;
            s = square;
            bool isNumber = is_number(s);
            int cost = atoi(s.c_str());
            
            if(square==' ' || square==startChar || square==endChar || (isNumber))
            {
                if(square==' ' || (isNumber))
                {
                    //domyslnie koszt akcji z jednego stanu do drugiego wynosi 1
                    //ewentualnie zmiana tej wartosci na wartosc liczbowa
                    
                    State tmp = State(k,row);
                    if(isNumber)
                        tmp.set_cost(cost);
                    else
                        tmp.set_cost(mazes[i].getDefCost());
                    states.push_back(tmp);
                }
                
                //ustawienie stanu startowego
                if(square==startChar)
                    states.push_back(State(k,row,false,true));
                
                //ustawienie stanu końcowego
                if(square==endChar)
                    states.push_back(State(k,row,true));
            }
            
            k++;
        }
        mazes[i].set_n(n);
        mazes[i].set_state(states);
    }
}

void generate_actions(std::vector<Maze> &mazes)
{
    for(size_t i=0;i<mazes.size();i++)
    {
        for(size_t j=0;j<mazes[i].getStates()->size();j++)
        {
            int last_letter = 0;
            
            State *ss = mazes[i].findByAdress(mazes[i].getStates()->at(j).get_name());
            
            std::string tmp="",tmp2="";
            int column=0;
            std::string adress=ss->get_name();
            
            //pobieramy wszystkie litery wiersza
            for(size_t ii=0;ii<ss->get_name().length();ii++)
            {
                
                if(adress[ii]>64 && adress[ii]<91)
                {
                    tmp+=adress[ii];
                    last_letter++;
                }
            }
            
            //pobieramy numer kolumny
            for(size_t is=last_letter;is<adress.length();is++)
                tmp2 +=adress[is];

            //spradzamy czy nie ma błędu
            bool isNumber = is_number(tmp2);

            if(isNumber)
                column = atoi(tmp2.c_str());
            
            //poszukiwanie wiersza wcześniejszego i późniejszego
            std::string tmp_1,tmp_2;
            tmp_1 = tmp; // wiersz w górę
            tmp_2 = tmp; // wiersz w wiersz później
            
            //generujemy następny i poprzedni wiersz(litery wiersza)
            for(int rrw = tmp.length()-1;rrw>-1;rrw--)
            {
                if(rrw==0 || tmp[rrw]!='Z')
                {
                    tmp_1[rrw]++;
                    if(tmp_1[rrw]>90)
                    {
                        for(size_t tst=0;tst<tmp_1.length();tst++)
                            tmp_1[tst] = 'A';
                        tmp_1+='A';
                    }
                    
                    tmp_2[rrw]--;
                    if(tmp_2[rrw]<65)
                    {
                        if(rrw>0)
                            tmp_2[rrw-1] = 'Z';
                        tmp_2[rrw]='A';
                        rrw +=1;
                        if(rrw<(int) tmp_2.length())
                        {
                            tmp_2[rrw]--;
                        }
                        else
                        {
                            std::string sssw=""; //zmienna pomocnicza
                            for(size_t tst=0;tst<tmp_2.length()-1;tst++)
                                sssw+=tmp_2[tst];
                            tmp_2 = sssw;
                        }
                    }
                    break;
                }
            }
            //następne ewiersze i kolumny znaleziono
            //czas na dodanie jako możliwych akcji, jeśli są w zbiorze stanów
            State *stmp;
            int cost=0;

            /*
             * szukamy,czy wygenerowane adresy są potencjalnymi
             * stanami w obrębie labiryntu
             */
            if(tmp_1!="")
            {
                stmp = mazes[i].findByAdress(tmp_1+std::to_string(column));

                if(stmp!=nullptr)
                {
                    cost = stmp->get_cost();
                    if(stmp==mazes[i].getStartState())
                        cost = mazes[i].getStartCost();

                    if(stmp==mazes[i].getEndState())
                        cost = mazes[i].getEndCost();

                    ss->add_map(stmp,cost);
                }
            }
            
            if(tmp_2!="")
            {
                stmp = mazes[i].findByAdress(tmp_2+std::to_string(column));
                if(stmp!=nullptr)
                {
                    cost = stmp->get_cost();
                    if(stmp==mazes[i].getStartState())
                        cost = mazes[i].getStartCost();

                    if(stmp==mazes[i].getEndState())
                        cost = mazes[i].getEndCost();

                    ss->add_map(stmp,cost);
                }
            }

            int column_1 = column+1;
            
            stmp = mazes[i].findByAdress(tmp+std::to_string(column_1));
            if(stmp!=nullptr)
            {
                cost = stmp->get_cost();
                if(stmp==mazes[i].getStartState())
                    cost = mazes[i].getStartCost();

                if(stmp==mazes[i].getEndState())
                    cost = mazes[i].getEndCost();

                ss->add_map(stmp,cost);
            }
            
            int column_2 = column-1;
            
            if(column_2>0)
            {
                stmp = mazes[i].findByAdress(tmp+std::to_string(column_2));
                if(stmp!=nullptr)
                {
                    cost = stmp->get_cost();
                    if(stmp==mazes[i].getStartState())
                        cost = mazes[i].getStartCost();

                    if(stmp==mazes[i].getEndState())
                        cost = mazes[i].getEndCost();

                    ss->add_map(stmp,cost);
                }
            }
        }
    }
}

//H1(n) = |endStateRow-currentStateRow|+|endStateColumn-currentStateColumn|
//inaczej mówiąc - najmniejsza odległóść między obecnym stanem a stanem końcowym
//bez ścian
int H1(State s,Maze *temp)
{
    int result =0;
    
    std::string row = s.getRow();
    int column = s.getColumn();
    
    std::string endRow = temp->getEndState()->getRow();
    int endColumn = temp->getEndState()->getColumn();
    
    //zamieniamy litery wiersza na wartość liczbowa
    //np. Z = 26, AA = 27, AB = 28 itd.
    int tmp = (endRow.length()-1)*25;
    for(size_t i=0;i<endRow.length();i++)
        tmp +=(endRow[i]-64);
    
    int tmp2 = (row.length()-1)*25;
    for(size_t i=0;i<row.length();i++)
        tmp2 +=(row[i]-64);
    
    //odległość nie może być mniejsza od zera
    int result_1 = tmp2-tmp;
    if(result_1<0)
        result_1 *=-1;
    
    int result_2 = endColumn-column;
    if(result_2<0)
        result_2 *=-1;
    
    //sumujemy odlegosc
    result = result_1+result_2;
    
    return result;
}

//H2(n) = ((wiersz-1)*liczba_kolumn)+numer_kolumny
//inaczej mówiąc numer komórki licząc od A1=1
int H2(State s,Maze *temp)
{
    int result = 0;
    
    std::string row = s.getRow();
    int column = s.getColumn();

    if(s.get_name()==temp->getEndState()->get_name())
        return 0;
    
    int tmp2 = (row.length()-1)*25;
    for(size_t i=0;i<row.length();i++)
        tmp2 +=(row[i]-64);

    tmp2 -=1;
    tmp2 *=temp->get_n();

    result = tmp2+column;
    /*int result =0;

    std::string row = s.getRow();
    int column = s.getColumn();

    std::string endRow = temp->getEndState()->getRow();
    int endColumn = temp->getEndState()->getColumn();

    //zamieniamy litery wiersza na wartość liczbowa
    //np. Z = 26, AA = 27, AB = 28 itd.
    int tmp = (endRow.length()-1)*25;
    for(size_t i=0;i<endRow.length();i++)
        tmp +=(endRow[i]-64);

    int tmp2 = (row.length()-1)*25;
    for(size_t i=0;i<row.length();i++)
        tmp2 +=(row[i]-64);

    //odległość nie może być mniejsza od zera
    int result_1 = tmp2-tmp;
    if(result_1<0)
        result_1 *=-1;

    int result_2 = endColumn-column;
    if(result_2<0)
        result_2 *=-1;
    result = std::max(result_1,result_2);
*/
    return result;
}

void load_settings(std::map<std::string,char> &settings)
{
    std::ifstream fileSettings;
    fileSettings.open(".\\settings.txt");
    std::string line;
    
    char wallChar ='X';
    char startChar = 'e';
    char endChar = 'j';
    char cost = '1';
    char alg_A = 'T';
    char alg_IDA = 'T';
    char heurestic_1 = 'T';
    char heurestic_2 = 'T';
    char out_cmd='T';
    char show_maze_file='F';
    char show_maze_cmd='F';
    char show_path_cmd='F';
    char startCost = '1';
    char endCost = '1';
    
    if(fileSettings.is_open())
    {
        while(!fileSettings.eof())
        {
            
            std::getline(fileSettings,line);
            if(line.find("wall_char")!=std::string::npos)
            {
                if(line[line.find_first_of("=\"")+3]=='\"')
                {
                    std::string tmp;
                    tmp = line[line.find_first_of('\"')+1];
                    bool isNumber=is_number(tmp);
                    if(!isNumber)
                        wallChar=line[line.find_first_of('\"')+1];
                }
            }
            
            if(line.find("start_char")!=std::string::npos)
            {
                if(line[line.find_first_of("=\"")+3]=='\"')
                {
                    std::string tmp;
                    tmp = line[line.find_first_of('\"')+1];
                    bool isNumber=is_number(tmp);
                    if(!isNumber)
                        startChar=line[line.find_first_of('\"')+1];
                }
            }
            
            if(line.find("end_char")!=std::string::npos)
            {
                if(line[line.find_first_of("=\"")+3]=='\"')
                {
                    std::string tmp;
                    tmp = line[line.find_first_of('\"')+1];
                    bool isNumber=is_number(tmp);
                    if(!isNumber)
                        endChar=line[line.find_first_of('\"')+1];
                }
            }
            
            if((startChar==wallChar) || (endChar==wallChar) || (startChar==endChar))
            {
                std::cout<<"Bledne ustawienia znakow labiryntu!\nLadowanie domyslnych ustawien\n";
                wallChar ='X';
                startChar = 'e';
                endChar = 'j';
            }
            
            if(line.find("default_cost")!=std::string::npos)
            {
                char tmp='/';
                if(line[line.find_first_of("=\"")+3]=='\"')
                    tmp=line[line.find_first_of('\"')+1];
                std::string c;
                c = tmp;
                bool isNumber=is_number(c);

                if(isNumber)
                {
                    cost = tmp;;
                }
            }

            if(line.find("start_cost")!=std::string::npos)
            {
                char tmp='/';
                if(line[line.find_first_of("=\"")+3]=='\"')
                    tmp=line[line.find_first_of('\"')+1];
                std::string c;
                c = tmp;
                bool isNumber=is_number(c);

                if(isNumber)
                {
                    startCost = tmp;;
                }
            }

            if(line.find("end_cost")!=std::string::npos)
            {
                char tmp='/';
                if(line[line.find_first_of("=\"")+3]=='\"')
                    tmp=line[line.find_first_of('\"')+1];
                std::string c;
                c = tmp;
                bool isNumber=is_number(c);

                if(isNumber)
                {
                    endCost = tmp;
                }
            }
            
            if(line.find("alg_A")!=std::string::npos)
            {
                if(line[line.find_first_of("=\"")+3]=='\"')
                {
                    if(line[line.find_first_of('\"')+1]=='t' || line[line.find_first_of('\"')+1]=='T' ||line[line.find_first_of('\"')+1]=='f'||line[line.find_first_of('\"')+1]=='F')
                        alg_A = line[line.find_first_of('\"')+1];
                }
            }
            
            if(line.find("alg_IDA")!=std::string::npos)
            {
                if(line[line.find_first_of("=\"")+3]=='\"')
                {
                    if(line[line.find_first_of('\"')+1]=='t' || line[line.find_first_of('\"')+1]=='T' ||line[line.find_first_of('\"')+1]=='f'||line[line.find_first_of('\"')+1]=='F')
                        alg_IDA = line[line.find_first_of('\"')+1];
                }
            }
            
            if(line.find("heurestic_1")!=std::string::npos)
            {
                if(line[line.find_first_of("=\"")+3]=='\"')
                {
                    if(line[line.find_first_of('\"')+1]=='t' || line[line.find_first_of('\"')+1]=='T' ||line[line.find_first_of('\"')+1]=='f'||line[line.find_first_of('\"')+1]=='F')
                        heurestic_1 = line[line.find_first_of('\"')+1];
                }
            }
            
            if(line.find("heurestic_2")!=std::string::npos)
            {
                if(line[line.find_first_of("=\"")+3]=='\"')
                {
                    if(line[line.find_first_of('\"')+1]=='t' || line[line.find_first_of('\"')+1]=='T' ||line[line.find_first_of('\"')+1]=='f'||line[line.find_first_of('\"')+1]=='F')
                        heurestic_2 = line[line.find_first_of('\"')+1];
                }
            }
            
            if(line.find("out_cmd")!=std::string::npos)
            {
                if(line[line.find_first_of("=\"")+3]=='\"')
                {
                    if(line[line.find_first_of('\"')+1]=='t' || line[line.find_first_of('\"')+1]=='T' ||line[line.find_first_of('\"')+1]=='f'||line[line.find_first_of('\"')+1]=='F')
                        out_cmd = line[line.find_first_of('\"')+1];
                }
            }
            
            if(line.find("show_maze_file")!=std::string::npos)
            {
                if(line[line.find_first_of("=\"")+3]=='\"')
                {
                    if(line[line.find_first_of('\"')+1]=='t' || line[line.find_first_of('\"')+1]=='T' ||line[line.find_first_of('\"')+1]=='f'||line[line.find_first_of('\"')+1]=='F')
                        show_maze_file = line[line.find_first_of('\"')+1];
                }
            }
            
            if(line.find("show_maze_cmd")!=std::string::npos)
            {
                if(line[line.find_first_of("=\"")+3]=='\"')
                {
                    if(line[line.find_first_of('\"')+1]=='t' || line[line.find_first_of('\"')+1]=='T' ||line[line.find_first_of('\"')+1]=='f'||line[line.find_first_of('\"')+1]=='F')
                        show_maze_cmd = line[line.find_first_of('\"')+1];
                }
            }
            
            if(line.find("show_path_cmd")!=std::string::npos)
            {
                if(line[line.find_first_of("=\"")+3]=='\"')
                {
                    if(line[line.find_first_of('\"')+1]=='t' || line[line.find_first_of('\"')+1]=='T' ||line[line.find_first_of('\"')+1]=='f'||line[line.find_first_of('\"')+1]=='F')
                        show_maze_cmd = line[line.find_first_of('\"')+1];
                }
            }
            
        }
    }
    else
    {
        std::cout<<"Brak pliku ustawien. Zaladowane zostana domyslne ustawienia.\n";
    }
    
    settings.insert(std::pair<std::string,char>("wallChar",wallChar));
    settings.insert(std::pair<std::string,char>("startChar",startChar));
    settings.insert(std::pair<std::string,char>("endChar",endChar));
    settings.insert(std::pair<std::string,char>("defaultCost",cost));
    settings.insert(std::pair<std::string,char>("startCost",startCost));
    settings.insert(std::pair<std::string,char>("endCost",endCost));
    settings.insert(std::pair<std::string,char>("alg_A",alg_A));
    settings.insert(std::pair<std::string,char>("alg_IDA",alg_IDA));
    settings.insert(std::pair<std::string,char>("heurestic_1",heurestic_1));
    settings.insert(std::pair<std::string,char>("heurestic_2",heurestic_2));
    settings.insert(std::pair<std::string,char>("out_cmd",out_cmd));
    settings.insert(std::pair<std::string,char>("show_maze_file",show_maze_file));
    settings.insert(std::pair<std::string,char>("show_maze_cmd",show_maze_cmd));
    settings.insert(std::pair<std::string,char>("show_path_cmd",show_path_cmd));
}

bool is_number(std::string c)
{

    std::string::const_iterator it = c.begin();
    while (it != c.end() && std::isdigit(*it)) ++it;
    return !c.empty() && it == c.end();

}

int load_arguments(int argc,char *argv[],std::string &inputDir, std::string &outputDir, int &repeats, long long int &depth)
{
    for(int i=1;i<argc;i++)
    {
        std::string tmp = argv[i];
        std::string result;
        if(argv[i][0]=='-')
        {
            size_t index_of_temp = std::string::npos;
            size_t index_of_temp2 = std::string::npos;
            size_t index_of_temp3 = std::string::npos;
            size_t index_of_temp4 = std::string::npos;

            std::string temp="input=";
            std::string temp2="output=";
            std::string temp3="repeats=";
            std::string temp4="depth=";

            index_of_temp = tmp.find(temp.c_str());
            index_of_temp2 = tmp.find(temp2.c_str());
            index_of_temp3 = tmp.find(temp3.c_str());
            index_of_temp4 = tmp.find(temp4.c_str());

            if(index_of_temp!=std::string::npos)
            {
                size_t fIndex, lIndex;
                if((fIndex = tmp.find_first_of('='))!=std::string::npos)
                {
                    lIndex = tmp.find_last_of('=');
                    if(lIndex!=std::string::npos)
                    {
                        result = tmp.substr(fIndex+1);
                        //result.erase(result.find_last_of('\"'));
                        result+="\\";
                        inputDir = result;
                    }
                }
            }

            if(index_of_temp2!=std::string::npos)
            {
                size_t fIndex, lIndex;
                if((fIndex = tmp.find_first_of('='))!=std::string::npos)
                {
                    lIndex = tmp.find_last_of('=');
                    if(lIndex!=std::string::npos)
                    {
                        result = tmp.substr(fIndex+1);
                        //result.erase(result.find_last_of('\"'));
                        result+="\\";
                        outputDir = result;
                    }
                }
            }

            if(index_of_temp3!=std::string::npos)
            {
                size_t fIndex, lIndex;
                if((fIndex = tmp.find_first_of('='))!=std::string::npos)
                {
                    lIndex = tmp.find_last_of('=');
                    if(lIndex!=std::string::npos)
                    {
                        result = tmp.substr(fIndex+1);
                        //result.erase(result.find_last_of('\"'));
                        repeats = atoi(result.c_str());
                    }
                }
            }
            if(index_of_temp4!=std::string::npos)
            {
                size_t fIndex, lIndex;
                if((fIndex = tmp.find_first_of('='))!=std::string::npos)
                {
                    lIndex = tmp.find_last_of('=');
                    if(lIndex!=std::string::npos)
                    {
                        result = tmp.substr(fIndex+1);
                        //result.erase(result.find_last_of('\"'));
                        depth = atoi(result.c_str());
                    }
                }
            }
        }
    }

    return 0;
}
