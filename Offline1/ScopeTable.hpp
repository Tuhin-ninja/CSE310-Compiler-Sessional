#include "SymbolInfo.hpp"
#include "OutputManager.hpp"


class ScopeTable{
    // This class implements the hash table --> The symbols in the same scope are arranged 
    // in the bucketList based on the appropriate hash function. For that reason we need a 
    // bucketList and to communicate between different ScopeTables i.e. we need to store 
    // the parentScope Table also. we will follow a  tree structure. 
    private : 
       SymbolInfo** scopeVariables; 
       ScopeTable* parentScope; 
       int size; 
       string id; 
       ofstream& logout; 
       
    public : 
    ScopeTable(int size,string id, ofstream& outputFile,ScopeTable* parentScope = nullptr) : logout(outputFile){
        this->size = size; 
        this->id = id; 
        this->parentScope = parentScope; 
        scopeVariables = new SymbolInfo*[size];
        for(int i=0;i<size;i++){
            scopeVariables[i] = nullptr; 
        }
    }

    unsigned long SDBMHash(string str) {
        long long hash = 0;
        unsigned int i = 0;
        unsigned int len = str.length();
    
        for (i = 0; i < len; i++)
        {
            hash = (str[i]) + (hash << 6) + (hash << 16) - hash;
        }
    
        return hash;
    }

    bool insert(SymbolInfo* symbolInfo){
        // stringstream ss(line); 
        // char delimeter = ' '; 
        // string word; 
        // int count = 0; 
        // string words[100];  // assuming max words 100. if found better solution, this should be modified... 
        // while(getline(ss,word,delimeter)){
        //     words[count++] = word;
        // }
        // string wordType = words[2];
        // cout<<wordType<<endl; 
        // string name = words[1];
        // string type; 

        // if (wordType == "FUNCTION") {
        //     stringstream ss;
        //     for (int i = 4; i < count; ++i) {
        //         ss << words[i];
        //         if (i != count - 1) ss << ",";
        //     }
        
        //     type = words[2] + "," + words[3] + "<==" + "("+ ss.str() + ")";  
        //     cout<<type<<endl;
        
        // } else if (wordType == "STRUCT" || wordType == "UNION") {
        //     stringstream ss;
        //     for(int i=3; i<count;i+=2){
        //         ss<<"("+words[i]+","+words[i+1]<<")";
        //         if(i != count-2) ss<<",";
        //     }

        //     type = wordType+ ","+"{" + ss.str() + "}";

        // } else{
        //     type = words[2]; 
        // }

        // SymbolInfo* symbolInfo = new SymbolInfo(name,type);
        // cout<<symbolInfo->to_string()<<endl; 
        // symbolTable->Insert(symbolInfo);



        long long index = SDBMHash(symbolInfo->getName()) % this->size; 
        if(index < 0) index*=-1;
        SymbolInfo* temp = scopeVariables[index];
        SymbolInfo* prev = nullptr; 
        int count = 0;
        if(temp == nullptr){
            scopeVariables[index] = symbolInfo;
            count++; 
            logout<<"\t Inserted in ScopeTable# "<<id<<" at position "<<index+1<<", "<<count<<endl;
            return true;
        }
        else{
            while(temp){
                count++;
                if(temp->getName() == symbolInfo->getName()){
                    logout<<"\t"<<symbolInfo->getName()<<" already exists in the current ScopeTable"<<endl;
                    return false; 
                }
                prev = temp; 
                temp = temp->getNextSymbol();
            }
            count++;
            prev->setNextSymbol(symbolInfo);
            logout<<"\t Inserted in ScopeTable# "<<id<<" at position "<<index+1<<", "<<count<<endl;
            return true;
        }
    }



    SymbolInfo* lookUp(string symbol){
        int index = SDBMHash(symbol) % this->size; 
        SymbolInfo* temp = scopeVariables[index]; 
        int count = 0;
        while(temp){
            count++;
            if(temp->getName() == symbol) {
                logout<<"\t"<<symbol <<" found in ScopeTable# "<<id<<" at position "<<index+1<<", "<<count<<endl;
                return temp; 
            }
            temp = temp->getNextSymbol(); 
        }

        return nullptr; 
    }



    bool Delete(string symbol){
        int index = SDBMHash(symbol) % this->size; 
        SymbolInfo* temp = scopeVariables[index]; 
        SymbolInfo* prev = nullptr; 
        int count = 0;
        while(temp){
            count++;
            if(temp->getName() == symbol){
                if(prev == nullptr){
                    scopeVariables[index] = temp->getNextSymbol(); 
                }
                else{
                    prev->setNextSymbol(temp->getNextSymbol()); 
                }
                delete temp;
                logout<<"\tDeleted "<<symbol<<" from the ScopeTable# "<<id<<" at position "<<index+1<<", "<<count<<endl; 
                return true; 
            }

            prev = temp; 
            temp = temp->getNextSymbol();
        }

        logout<<"\tNot found in the current ScopeTable"<<endl;
        return false; 
    }



    void printScopeTable(int tab=0){
        int tabs = tab;
        while(tabs--){
            logout<<"\t";
        }
        logout<<"\tScopeTable# "<<id<<endl; 
        for(int i=0; i<this->size; i++){
            tabs = tab;
            while(tabs--){
                logout<<"\t";
            }
            logout<<"\t"<<i+1<<"--->"<<" ";
            printSingleList(i);
            logout<<endl; 
        }
    }

    void printSingleList(int index){
        SymbolInfo* temp = scopeVariables[index]; 
        while(temp){
            logout<<temp->to_string()<<" ";
            temp = temp->getNextSymbol();
        }
    }


    void setID(string id){
        this->id = id; 
    }

    string getID(){
        return this->id; 
    }

    void setParentScope(ScopeTable* parentScope){
        this->parentScope = parentScope; 
    }

    ScopeTable* getParentScope(){
        return parentScope;
    }


    ~ScopeTable(){
        for(int i=0; i<this->size; i++){
            SymbolInfo* temp = scopeVariables[i]; 
            while(temp){
                SymbolInfo* toDelete = temp; 
                temp = temp->getNextSymbol(); 
                delete toDelete; 
            }
        }
        delete[] scopeVariables;
    }

}; 


