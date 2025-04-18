#include "SymbolTable.hpp"


class Compiler{
    private : 
       ifstream in; 
       ofstream out; 
       SymbolTable* symbolTable; 
       int commandCount;
    public : 
    Compiler(string inputFile, string outputFile){
        in.open(inputFile); 
        out.open(outputFile); 
        this->commandCount = 0;
        if(!in.is_open()){
            cout<<"Failed to open inputFile"<<endl;
        }
        else if(!out.is_open()){
            cout<<"Failed to open outputFile"<<endl;
        }
        else {
            string line; 
            getline(this->in,line); 
            symbolTable = new SymbolTable(stoi(line));
            out<<"\tScopeTable# "<<symbolTable->getCurrentScope()->getID()<<" created!"<<endl;
        }
    }


    void Insertion(string line){
        stringstream ss(line); 
        char delimeter = ' '; 
        string word; 
        int count = 1; 
        string words[100];  // assuming max words 100. if found better solution, this should be modified... 
        while(getline(ss,word,delimeter)){
            words[count++] = word;
        }

        string wordType = words[2];

        if (wordType == "FUNCTION") {
            string name = words[1];
        
            // Join the rest of the words from words[3] to the end, separated by commas
            stringstream ss;
            for (int i = 3; i < count; ++i) {
                ss << words[i];
                if (i != count - 1) ss << ",";
            }
        
            string type = words[2] + "," + ss.str();  // FUNCTION,int,float,... etc.
            cout << "Name: " << name << endl;
            cout << "Type: " << type << endl;
        
        } else if (wordType == "STRUCT") {
            // handle struct
        } else if (wordType == "UNION") {
            // handle union
        }
        
    }

    void compile(){
        string line; 
        while(getline(in,line)){
            commandCount++;
            out<<"Cmd "<<commandCount<<": "<<line<<endl; 
            switch(line[0]){
                case 'I' : Insertion(line);
                break;
            }
        }

    }

    ~Compiler(){
        cout<<"Closing files!"<<endl;
        in.close();
        out.close();
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

}