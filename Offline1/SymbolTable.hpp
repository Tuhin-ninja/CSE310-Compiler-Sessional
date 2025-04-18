#include "ScopeTable.hpp"

class SymbolTable{
    private : 
       ScopeTable* currentScopeTable; 
       int size; 
       int scopeCounter; 
    
    public : 
       SymbolTable(int size){
        this->size = size; 
        this->scopeCounter = 1; 
        this->currentScopeTable = new ScopeTable(size,to_string(scopeCounter)); 
       }


       bool Insert(SymbolInfo* symbolInfo){
        return currentScopeTable->insert(symbolInfo);
       }

       void EnterScope(){
        this->scopeCounter++; 
        ScopeTable* newScopeTable = new ScopeTable(size,to_string(scopeCounter),currentScopeTable); 
        currentScopeTable = newScopeTable; 
        cout<<"Scope Table "<<"#"<<currentScopeTable->getID()<<" created!"<<endl;
       }


       void ExitScope(){
        ScopeTable* temp = currentScopeTable; 
        currentScopeTable = currentScopeTable->getParentScope(); 
        delete temp; 
       }


       void printAllScopes(){
        ScopeTable* temp = currentScopeTable; 
        while(temp){
            cout<<"ScopeTable "<<temp->getID()<<endl; 
            temp = temp->getParentScope();
        }
       }


       ScopeTable* getCurrentScope(){
        return this->currentScopeTable; 
       }

};

