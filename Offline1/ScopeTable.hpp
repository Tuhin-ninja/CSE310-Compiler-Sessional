#include "SymbolInfo.hpp"


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
       
    public : 
    ScopeTable(int size,string id, ScopeTable* parentScope = nullptr){
        this->size = size; 
        this->id = id; 
        this->parentScope = parentScope; 
        scopeVariables = new SymbolInfo*[size];
        for(int i=0;i<size;i++){
            scopeVariables[i] = nullptr; 
        }
    }

    unsigned int SDBMHash(string str) {
        unsigned int hash = 0;
        unsigned int i = 0;
        unsigned int len = str.length();
    
        for (i = 0; i < len; i++)
        {
            hash = (str[i]) + (hash << 6) + (hash << 16) - hash;
        }
    
        return hash;
    }

    bool insert(SymbolInfo* symbolInfo){
        int index = SDBMHash(symbolInfo->getName()) % this->size; 
        cout<<index<<endl; 
        SymbolInfo* temp = scopeVariables[index];
        if(temp == nullptr){
            scopeVariables[index] = symbolInfo;
            cout<<"New Bucket List created and Insertion was successful!"<<endl; 
            return true; 
        }

        while(temp->getNextSymbol() != nullptr){
            if(temp->getName() == symbolInfo->getName()){
                cout<<"The variable with the same name already exists!"<<endl;
                return false; 
            }
            temp = temp->getNextSymbol(); 
        }

        if(temp->getName() == symbolInfo->getName()){
            cout<<"The variable with the same name already exists!"<<endl;
            return false; 
        }

        temp->setNextSymbol(symbolInfo); 
        cout<<"Insertion Successful"<<endl;
        return true; 
    }



    SymbolInfo* lookUp(string symbol){
        int index = SDBMHash(symbol) % this->size; 
        SymbolInfo* temp = scopeVariables[index]; 

        while(temp){
            if(temp->getName() == symbol) return temp; 
            temp = temp->getNextSymbol(); 
        }

        cout<<"Couldnt find the variable mentioned!"<<endl; 
        return nullptr; 
    }



    bool Delete(string symbol){
        int index = SDBMHash(symbol) % this->size; 
        SymbolInfo* temp = scopeVariables[index]; 
        SymbolInfo* prev = nullptr; 
        while(temp){
            if(temp->getName() == symbol){
                if(prev == nullptr){
                    scopeVariables[index] = temp->getNextSymbol(); 
                }
                else{
                    prev->setNextSymbol(temp->getNextSymbol()); 
                }
                delete temp;
                return true; 
            }

            prev = temp; 
            temp = temp->getNextSymbol();
        }

        return false; 
    }



    void printScopeVariables(){
        for(int i=0; i<this->size; i++){
            cout<<"bucket["<<i<<"]"<<" : ";
            printSingleList(i);
            cout<<endl; 
        }
    }

    void printSingleList(int index){
        SymbolInfo* temp = scopeVariables[index]; 
        while(temp){
            cout<<"("<<temp->getName()<<" : "<<temp->getType()<<")"; 
            temp = temp->getNextSymbol(); 
            if(temp){
                cout<<"->";
            }
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


