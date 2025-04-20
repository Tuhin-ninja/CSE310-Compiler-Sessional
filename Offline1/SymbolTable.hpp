#include "ScopeTable.hpp"
#include "hash_functions.hpp"

class SymbolTable{
    private : 
       ScopeTable* currentScopeTable; 
       int size; 
       ofstream& logout;
       int scopeCounter; 
       string hashFunctionName;
    
    public : 
       SymbolTable(int size, ofstream& outputFile, string hashFunctionName) : logout(outputFile){
        this->size = size; 
        this->scopeCounter = 1; 
        this->currentScopeTable = new ScopeTable(size,to_string(scopeCounter),logout, nullptr, hashFunctionName); 
        this->hashFunctionName = hashFunctionName;
       }


       bool Insert(SymbolInfo* symbolInfo){
        return currentScopeTable->insert(symbolInfo);
       }


       bool Remove(string symbol){
        return currentScopeTable->Delete(symbol); 
       }

       SymbolInfo* LookUp(string symbol){
        SymbolInfo* symbolInfo; 
        ScopeTable* temp = currentScopeTable; 
        while(temp){
            symbolInfo = temp->lookUp(symbol); 
            if(symbolInfo != nullptr){
                return symbolInfo;
            }
            temp = temp->getParentScope();
        }
        symbol = "\'" + symbol + "\'";
        logout<<"\t"<<symbol<<" not found in any of the ScopeTables"<<endl;
        return nullptr; 
       }

       void EnterScope(){
        this->scopeCounter++; 
        ScopeTable* newScopeTable = new ScopeTable(size,to_string(scopeCounter), logout,currentScopeTable); 
        currentScopeTable = newScopeTable; 
        logout<<"\tScopeTable# "<<currentScopeTable->getID()<<" created"<<endl; 
       }


       void ExitScope(){
        if(currentScopeTable->getParentScope() == nullptr) {
            logout<<"\tCannot delete root ScopeTable"<<endl;
            return; 
        } 
        ScopeTable* temp = currentScopeTable; 
        currentScopeTable = currentScopeTable->getParentScope(); 
        logout<<"\tScopeTable# "<<temp->getID()<<" removed"<<endl; 
        delete temp; 
        return; 
       }


       void printCurrentScope(){
        currentScopeTable->printScopeTable();
       }


       void printAllScopes(){
        ScopeTable* temp = currentScopeTable; 
        int count = 0;
        while(temp != nullptr){
            temp->printScopeTable(count);
            temp = temp->getParentScope();
            count++;
        }
       }


       ScopeTable* getCurrentScope(){
        return this->currentScopeTable; 
       }


       ~SymbolTable(){
        ScopeTable* temp = currentScopeTable;
        while(temp != nullptr){
            ScopeTable* parent = temp->getParentScope(); 
            logout<<"\tScopeTable# "<<temp->getID()<<" removed"<<endl;
            delete temp; 
            temp = parent; 
        }
    }
    

};

