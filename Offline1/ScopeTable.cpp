#include "SymbolInfo.h"


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
    ScopeTable(int size, ScopeTable* parentScope = nullptr){
        this->size = size; 
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

}; 



int main(){
    ScopeTable* scopeTable = new ScopeTable(5);
    SymbolInfo* symbolInfo1 = new SymbolInfo("a","int");
    SymbolInfo* symbolInfo2 = new SymbolInfo("f","int");
    SymbolInfo* symbolInfo3 = new SymbolInfo("d","int");
    SymbolInfo* symbolInfo4 = new SymbolInfo("d","int");
    scopeTable->insert(symbolInfo1);
    scopeTable->insert(symbolInfo2); 
    scopeTable->insert(symbolInfo3);
    scopeTable->insert(symbolInfo4);
    scopeTable->printScopeVariables();
}