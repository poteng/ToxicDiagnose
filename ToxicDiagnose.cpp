
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <queue>
#include <fstream>

using namespace std;




struct Variable
{
    string name;
    string value;
    bool instantiate;
};

struct Conclusion
{
    string name;
    string value;
    string possibleAnswer;
};

//This struct converts "If name1 == value1 then nameConclusion = D"
struct Question
{
    string name1;
    string value1;
};


//input knowledge base from file
bool openKnowledgeBaseFile(fstream&, const char*);

//display 3 lists
void display_clauseList(Question*, int);
void display_conclusion_list(Conclusion*, int);
void display_variable_list(Variable*, int);

//build 3 lists
void build_clause_list(fstream&, Question*, Conclusion*, int);
int build_variable_list(Variable*, Question*, int, Conclusion*, int);

//check if a variable is in variable list or conclusion list
//check_variable also asks for information from the user
int check_conclusion (string, Conclusion*, int);
int check_variable(string, Variable*, int);

//check current conditions to see if more questions are not needed
bool check_stop_path(Variable*, int, Question*, int);

//get conclusion by giving the conclusion name and 3 lists and their size
string get_conclusion(string, Conclusion*, Question*, Variable*, int, int, int);

string backward_chaining();





//=======================================

void IdentifyPoison();
void Treatment();
void InitializeList();
void InitializeList2();
void Instantiate(string question, string &value, bool instantiate);
void ExecRule(int rule_num);
void forward_chaining(string);




struct Variable_f
{
    int number;
    string name;
    string value;
    string question;
    bool instantiate;
};

const int conclist_size = 30,
          varlist_size = 14,
          varlist2_size = 1,
          clausevarlist_size = 330,
          clausevarlist2_size = 64,
          statement_size = 30,
          statement2_size = 15;

Variable_f varlist[varlist_size];
Variable_f conclist[conclist_size];
Variable_f clausevarlist[clausevarlist_size];
Variable_f varlist2[varlist2_size];
Variable_f clausevarlist2[clausevarlist2_size];

int conc_counter = 0,
    clause_counter = 0,
    clause_pointer = 0;

Variable_f treatment;

string arsenic_treat = "Hemodialysis and Blood Transfusion.",
       cyanide_treat = "Use Cyanide Antidote kit, Administer 100 percent oxygen, Use Benzodiazepines for muscle spasms.",
       strychnine_treat = "Decontaminate patient's blood using active charcoal.",
       tetrodotoxin_treat = "Change patient's clothes and Monitor him for Hypotension, respiratory depression, low blood sugar.",
       nerve_agents_treat = "Give Antidote injection and ventilatory support and administer oxygen.",
       chlorine_treat = "Change patient's clothes, wash body with soap and water and provide supportive medical care.",
       arsine_treat = "Blood transfusion and IV fluids to be given.  Dialysis may be needed to clean the blood.",
       ricin_treat = "IV fluids to be give to patient. Benzodiazepine to treat seizures and flush stomach with activated charcoal.",
       colchicine_treat = "Adminster charcoal as slurry. Begin IV fluids. Gastric lavage within one hour of ingestion.",
       sulfur_mustard_gas_treat = "Administer oxygen, assist ventilation and provide artificial respiration if required.",
       rattlesnake_treat = "If the bite is serious, then administer pit viper antivenom.  Otherwise clean the wound, treat the symptoms, and give a tetanus vaccine.",
       copperhead_mocassin_treat = "If the bite is serious, then administer pit viper antivenom.  Otherwise clean the wound, treat the symptoms, and give a tetanus vaccine.",
       coral_snake_treat = "Administer Coral Snake Antivenom.",
       black_widow_treat = "Administer Black Widow Antitoxin.",
       brown_recluse_treat = "There is no antitoxin.  Treat the symptoms.  Use antibiotics in case of infection.  Use braces if the bite is near a joint.";


Variable_f poison;

int main()
{
    string result = "";
    string inputUser = "";
    do
    {
        result = backward_chaining();

        if (result != "wrong")
        {
            forward_chaining(result);
        }
        cout << "Would you like to try another search? ";
        cin >> inputUser;
    } while (inputUser == "Yes");
    cout << "\n\nProgram ends. Thanks for using.\n";
    return 0;
}


string backward_chaining()
{
    const char *FILE_NAME_STA = "statement.txt";

    int get;
    const int num_statement = 30;
    const int num_clause_per_statement = 10;
    const int num_variable = num_statement * num_clause_per_statement;
    int num_variable_real = 0;

    Variable variable_list[num_variable];
    Conclusion conclusion_list[num_statement];

    fstream knowledgeFile;
    Question clause_list[num_statement * num_clause_per_statement];

    bool displayList = false;
    string inputUser = "";
    string result = "";

    //set default values for lists
    for (int i = 0; i < num_variable; i++)
    {
        variable_list[i].name = "";
        variable_list[i].value = "";
        variable_list[i].instantiate = false;
    }
    for (int i = 0; i < num_statement * num_clause_per_statement; i++)
    {
        clause_list[i].name1 = "";
        clause_list[i].value1 = "";
    }

    //open file
    if (openKnowledgeBaseFile(knowledgeFile, FILE_NAME_STA) == false)
    {
            cout << "There is no file existing. Please check." << endl;
            return "wrong";
    }


    build_clause_list(knowledgeFile, clause_list, conclusion_list, num_clause_per_statement);
    num_variable_real = build_variable_list(variable_list, clause_list, num_statement * num_clause_per_statement,
                                            conclusion_list, num_statement);

    display_clauseList(clause_list, num_statement * num_clause_per_statement);
    display_variable_list(variable_list, num_variable_real);
    display_conclusion_list(conclusion_list, num_statement);


    //get conclusion from the user
    cout << "Press 1 to find a poison, or enter a conclusion to find:  ";
    cin >> inputUser;

    if (inputUser == "1") { inputUser = "Poison"; }
    result = get_conclusion(inputUser, conclusion_list, clause_list, variable_list,
                            num_statement, num_clause_per_statement, num_variable_real);

    if (result == "No Match")
    {
        cout << "There is no conclusion for this condition." << endl;
    }
    else //give out the result
    {
        cout << "The result for " << inputUser << " is " << result << endl;
    }

    return result;
    cout << "\n\nBackward Chaining Ends.\n\n\n";
}



bool openKnowledgeBaseFile(fstream& file, const char* name)
{
    file.open(name, ios::in);
    if (!file)
    {
        cout << "Knowledge base file does not exist." << endl;
        return false;
    }
    return true;
}

void display_clauseList(Question* listQuestion, int size)
{
    cout << "\n====Building clause list...==== \nClause list:\n\n";
    for (int i = 0; i < size; i++)
    {
        cout << "clause number "<< i << "  " <<listQuestion[i].name1
            << " = " << listQuestion[i].value1 << endl;
        if ( (i % 100 == 0) && (i != 0) )
        {
            cout << "HIT RETURN KEY TO CONTINUE";
            cin.get();
        }
    }
}

void display_conclusion_list(Conclusion* listConclusion, int sizeList)
{
    cout << "\nConclusion list:\n\n";
    for (int i = 0; i < sizeList; i++)
    {
        cout << i << "  " << listConclusion[i].name << " = " << listConclusion[i].value << endl;
    }
}

//It gets a fstream object and put information to clause list
void build_clause_list(fstream& file, Question* listQuestion, Conclusion* listConclusion,
                       int clause_per_statement)
{
    int sizeFile = 0;
    int countLine = 0;
    int countStatement = 0;
    int countClause = 0;
    string bufferInput;

    file >> sizeFile;
    cout << "Inputing file. The file size is " << sizeFile << endl;

    while (countLine < sizeFile)
    {
        file >> bufferInput;
        if (bufferInput == "then") //then part
        {
            file >> listConclusion[countStatement].name; //answer
            file >> listConclusion[countStatement].possibleAnswer; //answer value
            countLine++; //as it reads one more line for "then"
            countStatement++;
            countClause = 0;
        }
        else //if part
        {
            listQuestion[countStatement * clause_per_statement + countClause].name1 = bufferInput;
            file >> listQuestion[countStatement * clause_per_statement + countClause].value1;
            countClause++;
        }
        countLine+=2;
    }
}



void display_variable_list(Variable* variable_list, int sizeVariable)
{
    cout << "\nPrinting variable list:" << endl;
    for (int i = 0; i < sizeVariable; i++)
    {
        cout << variable_list[i].name << " = " << variable_list[i].value << endl;
    }
};


//return the index of conclusion if the given variable name is one of conclusions
//otherwise, return -1
int check_conclusion (string nameConclusion, Conclusion* listConclusion, int sizeConclusion)
{
    for (int i = 0; i < sizeConclusion; i++)
    {
        if (listConclusion[i].name == nameConclusion) { return i; }
    }
    return -1;
}


//It build the variable list based on clause variable list.
int build_variable_list(Variable* listVariable, Question* listClause, int sizeClause,
                        Conclusion* listConclusion, int sizeConclusion)
{
    int sizeVariableReal = 0;
    for (int i = 0; i < sizeClause; i++)    //go through clause list
    {
        if (listClause[i].name1 != "")      //if it's not a empty clause
        {
            //don't put it if it's a conclusion
            if ( check_conclusion(listClause[i].name1, listConclusion, sizeConclusion) == -1 )
            {
                for (int j = 0; j < sizeClause; j++)
                {
                    //only put every variable once
                    if (listVariable[j].name == listClause[i].name1)
                    {
                        break;
                    }
                    else if (listVariable[j].name == "")
                    {
                        listVariable[j].name = listClause[i].name1;
                        sizeVariableReal = j + 1;
                        break;
                    }
                }
            }
        }
    }
    return sizeVariableReal;
}


//check the given variable is initiated or not, and ask the user if not
//return the index of that variable for following use
int check_variable(string nameVariable, Variable* listVariable, int sizeVariable)
{
    for (int i = 0; i < sizeVariable; i++)
    {
        if (listVariable[i].name == nameVariable)
        {
            if (listVariable[i].instantiate == false)
            {
                cout << "\nDoes the patient have \"" << listVariable[i].name << "\"? (Yes/No)" << endl;
                ///////////////////need to deal with input
                cin >> listVariable[i].value;
                listVariable[i].instantiate = true;
            }
            return i;
        }
    }
    cout << "Why here???" << endl;
    return -1;
}

bool check_stop_path(Variable* listVariable, int indexVariable, Question* listClause, int indexClause)
{
    if (listVariable[indexVariable].value != listClause[indexClause].value1) { return true; }
    else { return false; }
}


//try to get the value of given conclusion
string get_conclusion(string conclusion, Conclusion* listConclusion, Question* listClause,
                       Variable* listVariable, int sizeConclusion, int clausePerStatement, int sizeVariable)
{
    int indexConclusion = -1;
    int indexVariable = -1;
    bool stopPath = false;
    //if the answer from the user is already against the current rule, stop it.

    cout << "Trying to get conclusion: " << conclusion << endl;


    //search conclusion to find which clause has it
    //num_statement is the size of conclusion list
    for (int i = 0; i < sizeConclusion; i++)
    {
        if (conclusion == listConclusion[i].name)
        {
            indexConclusion = i;
            cout << "Checking conclusion#" << indexConclusion<<endl;

            if (indexConclusion == -1)
            {
                return "No Match";
            }

            else //examine this one conclusion and see if we can get the possible answer
            {
                //Find clause which can generate the variable/conclusion the last clause need
                for (int i = indexConclusion * clausePerStatement; i < (indexConclusion + 1) * clausePerStatement; i++)
                {
                    if ( listClause[i].name1 != "" )
                    {
                        //check if variables are initiated or any of them is conclusion
                        int indexSubConclusion = 0;
                        indexSubConclusion = check_conclusion(listClause[i].name1, listConclusion, sizeConclusion);
                        if (indexSubConclusion != -1) //it's a conclusion
                        {
                            listConclusion[indexSubConclusion].value = get_conclusion(listClause[i].name1,
                                listConclusion, listClause, listVariable, sizeConclusion, clausePerStatement, sizeVariable);
                            cout << "Sub conclusion " << listConclusion[indexSubConclusion].name
                                << " now is " <<listConclusion[indexSubConclusion].value << endl;
                        }
                        else if (listClause[i].name1 != "") //it's a variable
                        {
                            indexVariable = check_variable(listClause[i].name1, listVariable, sizeVariable);

                            if (check_stop_path(listVariable, indexVariable, listClause, i) == true) { break; }
                        }
                    }
                }

                cout << "Finished with conclusion " << conclusion << endl;

                //go through all conditions
                bool satisfied = true;
                for (int i = indexConclusion * clausePerStatement; i < (indexConclusion + 1) * clausePerStatement; i++)
                {
                    if (listClause[i].name1 != "")
                    {
                        //check conclusions
                        for (int j = 0; j < sizeConclusion; j++)
                        {
                            //Need to care about not choosing the same name conclusions with empty values
                            if ( (listConclusion[j].value != "") && (listClause[i].name1 == listConclusion[j].name)
                                && (listConclusion[j].value != listClause[i].value1) )
                            {
                                satisfied = false;
                            }
                        }
                        //check variables
                        for (int j = 0; j < sizeVariable; j++)
                        {
                            if ( (listClause[i].name1 == listVariable[j].name) && (listVariable[j].value != listClause[i].value1) )
                            {
                                //cout <<"clause = "<<listClause[i].name1<<" variable = "<<listVariable[j].name<<endl;
                                //cout << "var = "<<listVariable[j].value << " but need to be "<<listClause[i].value1 << endl;
                                satisfied = false;
                            }
                        }
                    }
                }
                if (satisfied == true) { return listConclusion[indexConclusion].possibleAnswer; }
            }
        }
    }
    return "No match";
}




//====================================================



void forward_chaining(string valuePoison)
{
    Variable_f Empty;
    Empty.name = "  ";

    for(int i = 0; i < clausevarlist_size; i++)
        clausevarlist[i] = Empty;

    for(int i = 0; i < clausevarlist2_size; i++)
        clausevarlist2[i] = Empty;

    poison.name = "Poison";
    poison.value = valuePoison;       // Enter the poison type here
    poison.instantiate = true;

    varlist2[0] = poison;

    for(int i = 0; i < clausevarlist2_size; i++)
    {
        if (i % 4 == 0)
            clausevarlist2[i] = poison;
    }

    InitializeList2();

    Treatment();
}


void Treatment()
{
    treatment.name = "Treatment";
    treatment.instantiate = false;

    clause_pointer = 0;
    queue<Variable_f> cond;
    cond.push(poison);

    bool stop = false;

    while (cond.empty() == false && treatment.instantiate == false)
    {
        for (int i = 0; i < clausevarlist2_size; i++)
        {
            if (i % 4 == 0)
                clause_pointer = i;
            if (clausevarlist2[i].name == cond.front().name)
            {
               for (int j = clause_pointer; j < clause_pointer + 4; j++)
               {
                  if (clausevarlist2[i].name != cond.front().name && clausevarlist2[i].name != "  " &&
                      clausevarlist2[i].instantiate == false)
                     Instantiate(clausevarlist2[i].question, clausevarlist2[i].value, clausevarlist2[i].instantiate);
               }
               ExecRule(i / 4 + 1);
            }
        }
        cond.pop();
    }
    if (treatment.instantiate == true)
        cout << endl << "Recommended Treatment(s):" << endl << treatment.value << endl;
}

void InitializeList()
{
    cout << "*** Conclusion List *" << endl;
    for(int i = 0; i < conclist_size; i++)
    {
        cout << "CONCLUSION " << (i + 1) << "  " << conclist[i].name << endl;
    }
    cout << "HIT RETURN KEY TO CONTINUE";
    cin.get();
    cout << endl;

    cout << "*** Variable List *" << endl;
    for (int i = 0; i < varlist_size; i++)
    {
        cout << "VARIABLE " << (i + 1) << "  " << varlist[i].name << endl;
    }
    cout << "HIT RETURN KEY TO CONTINUE";
    cin.get();
    cout << endl;


    cout << "*** Clause Variable List *" << endl;
    for(int i = 0; i < statement_size ; i++)
    {
        cout << "** CLAUSE " << (i+1) << endl;
        for(int j = 0; j < 11; j++)
        {
            cout << "VARIABLE " << (j + 1) << "  "
                 << clausevarlist[(i * 11) + j].name << endl;
        }
        if (i < 2)
        {
            cout << "HIT RETURN KEY TO CONTINUE";
            cin.get();
        }
    }
    cout << endl;
}

void InitializeList2()
{

    cout << "*** Variable List *" << endl;
    for (int i = 0; i < varlist2_size; i++)
    {
        cout << "VARIABLE " << (i + 1) << "  " << varlist2[i].name << endl;
    }
    cout << "HIT RETURN KEY TO CONTINUE";
    cin.get();
    cout << endl;


    cout << "*** Clause Variable List *" << endl;
    for(int i = 0; i < statement2_size ; i++)
    {
        cout << "** CLAUSE " << (i + 1) << endl;
        for(int j = 0; j < 4; j++)
        {
            cout << "VARIABLE " << (j + 1) << "  "
                 << clausevarlist2[(i * 4) + j].name << endl;
        }
        if (i < 2)
        {
            cout << "HIT RETURN KEY TO CONTINUE";
            cin.get();
        }
    }
    cout << endl;
}

void Instantiate(string question, string &value, bool instantiate)
{
    cout << question << "  ";
    cin >> value;
    cout << endl;

    instantiate = true;
}

void ExecRule(int rule_num)
{
    switch(rule_num)
    {
        case 1: if (poison.value == "Severe_exposure_to_Arsenic" || poison.value == "Moderate_exposure_to_Arsenic")
                {
                   treatment.value = arsenic_treat;
                   treatment.instantiate = true;
                }
                break;

        case 2: if (poison.value == "Severe_exposure_to_Potassium_Cyanide" || poison.value == "Moderate_exposure_to_Potassium_Cyanide" || poison.value == "Sodium_Cyanide")
                {
                   treatment.value = cyanide_treat;
                   treatment.instantiate = true;
                }
                break;

        case 3: if (poison.value == "Strychnine")
                {
                    treatment.value = strychnine_treat;
                    treatment.instantiate = true;
                }
                break;

        case 4: if (poison.value == "Tetrodotoxin")
                {
                   treatment.value = tetrodotoxin_treat;
                   treatment.instantiate = true;
                }
                break;

        case 5: if (poison.value == "Severe_exposure_to_Nerve_Agents" || poison.value == "Moderate_exposure_to_Nerve_Agents")
                {
                   treatment.value = nerve_agents_treat;
                   treatment.instantiate = true;
                }
                break;

        case 6: if (poison.value == "Moderate_exposure_to_Chlorine_gas" || poison.value == "Severe_exposure_to_Chlorine_gas")
                {
                   treatment.value = chlorine_treat;
                   treatment.instantiate = true;
                }
                break;

        case 7: if (poison.value == "Severe_Arsine_gas_exposure" || poison.value == "Moderate_Arsine_gas_exposure")
                {
                   treatment.value = arsine_treat;
                   treatment.instantiate = true;
                }
                break;

        case 8: if (poison.value == "Ricin")
                {
                   treatment.value = ricin_treat;
                   treatment.instantiate = true;
                }
                break;

        case 9: if (poison.value == "Colchicine")
                 {
                    treatment.value = colchicine_treat;
                    treatment.instantiate = true;
                 }
                 break;

        case 10: if (poison.value == "Severe_inhalation_of_Sulfur_Mustard" || poison.value == "Moderate_inhalation_of_Sulfur_Mustard")
                 {
                    treatment.value = sulfur_mustard_gas_treat;
                    treatment.instantiate = true;
                 }
                 break;

        case 11: if (poison.value == "Rattlesnake")
                 {
                    treatment.value = rattlesnake_treat;
                    treatment.instantiate = true;
                 }
                 break;

        case 12: if (poison.value == "Copperhead/Moccasin")
                 {
                    treatment.value = copperhead_mocassin_treat;
                    treatment.instantiate = true;
                 }
                 break;

        case 13: if (poison.value == "Coral_Snake")
                 {
                    treatment.value = coral_snake_treat;
                    treatment.instantiate = true;
                 }
                 break;

        case 14: if (poison.value == "Black_Widow")
                 {
                    treatment.value = black_widow_treat;
                    treatment.instantiate = true;
                 }
                 break;

        case 15: if (poison.value == "Brown_Recluse")
                 {
                    treatment.value = brown_recluse_treat;
                    treatment.instantiate = true;
                 }
                 break;
    }
}
