#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "SymbolTable.hpp"
#include "OutputManager.hpp"

using namespace std;

class Compiler{
    private : 
       ifstream in; 
       SymbolTable* symbolTable; 
       int commandCount;
    public : 
    Compiler(string inputFile, string outputFile){
        in.open(inputFile); 
        initializeLogout(outputFile);
        this->commandCount = 0;
        if(!in.is_open()){
            cout<<"Failed to open inputFile"<<endl;
        }
        else if(!logout.is_open()){
            cout<<"Failed to open outputFile"<<endl;
        }
        else {
            string line; 
            getline(this->in,line); 
         
            symbolTable = new SymbolTable(stoi(line), logout);
            logout<<"\tScopeTable# "<<symbolTable->getCurrentScope()->getID()<<" created!"<<endl;
        }
    }


    void Insertion(string line){
        stringstream ss(line); 
        char delimeter = ' '; 
        string word; 
        int count = 0; 
        string words[100];  // assuming max words 100. if found better solution, this should be modified... 
        while(getline(ss,word,delimeter)){
            words[count++] = word;
        }
        string wordType = words[2];
        cout<<wordType<<endl; 
        string name = words[1];
        string type; 

        if (wordType == "FUNCTION") {
            stringstream ss;
            for (int i = 4; i < count; ++i) {
                ss << words[i];
                if (i != count - 1) ss << ",";
            }
        
            type = words[2] + "," + words[3] + "<==" + "("+ ss.str() + ")";  
            cout<<type<<endl;
        
        } else if (wordType == "STRUCT" || wordType == "UNION") {
            stringstream ss;
            for(int i=3; i<count;i+=2){
                ss<<"("+words[i]+","+words[i+1]<<")";
                if(i != count-2) ss<<",";
            }

            type = wordType+ ","+"{" + ss.str() + "}";

        } else{
            type = words[2]; 
        }

        SymbolInfo* symbolInfo = new SymbolInfo(name,type);
        cout<<symbolInfo->to_string()<<endl; 
        symbolTable->Insert(symbolInfo);
        
    }


    void Lookup(string line){
        stringstream ss(line); 
        char delimeter = ' '; 
        string word; 
        int count = 0; 
        string words[100];  // assuming max words 100. if found better solution, this should be modified... 
        while(getline(ss,word,delimeter)){
            words[count++] = word;
        }
        string name = words[1];
        if(count != 2){
            logout<<"\tNumber of parameters mismatch for the command L"<<endl; 
            return; 
        } 
        SymbolInfo* symbolInfo = symbolTable->LookUp(name);
        if(symbolInfo != nullptr){
            cout<<symbolInfo->to_string()<<endl; 
        }
        else{
            cout<<"Not found!"<<endl;
        }
    }


    void Delete(string line){
        stringstream ss(line); 
        char delimeter = ' '; 
        string word; 
        int count = 0; 
        string words[100]; 
        while(getline(ss,word,delimeter)){
            words[count++] = word; 
        }

        string name = words[1]; 
        symbolTable->Remove(name);

    }



    void Print(string line){
        stringstream ss(line); 
        char delimeter = ' '; 
        string word; 
        int count = 0; 
        string words[100]; 
        while(getline(ss,word,delimeter)){
            words[count++] = word; 
        }

        if(words[1] == "C"){
            symbolTable->printCurrentScope();
        }

        if(words[1] == "A"){
            symbolTable->printAllScopes();
        }
    }








    void compile(){
        string line; 
        while(getline(in,line)){
            commandCount++;
            logout<<"Cmd "<<commandCount<<": "<<line<<endl; 
            switch(line[0]){
                case 'I' : Insertion(line);
                           break;
                case 'L' : Lookup(line);
                           break;
                case 'D' : Delete(line); 
                           break; 
                case 'P' : Print(line);
                           break; 
                case 'S' : symbolTable->EnterScope();
                           break; 
                case 'E' : symbolTable->ExitScope();
                           break;
                // case 'Q' : logout.close();
                //            exit(true);
                
            }
        }

    }

    ~Compiler(){
        cout<<"Closing files!"<<endl;
        in.close();
        logout.close();
        if(symbolTable) {
            delete symbolTable;
        }
    }

};

int main(){
    string inputFile, outputFile; 
    cout<<"Enter the input file Name : ";
    cin>>inputFile;
    cout<<"Enter the output file Name : ";
    cin>>outputFile; 
    Compiler* compiler = new Compiler(inputFile,outputFile); 
    compiler->compile();
    delete compiler;
    return 0;

}