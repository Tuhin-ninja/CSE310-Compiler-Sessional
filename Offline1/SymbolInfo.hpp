#include<bits/stdc++.h>
using namespace std; 

class SymbolInfo{
    private : 
        string name;
        string type;
        SymbolInfo* nextSymbol; 

    public : 
      SymbolInfo(string name="", string type="", SymbolInfo* nextSymbol = nullptr){
        this->name = name; 
        this->type = type; 
        this->nextSymbol = nextSymbol; 
      }

      string getName(){
        return name; 
      }
      string getType(){
        return type; 
      }
      SymbolInfo* getNextSymbol(){
        return nextSymbol; 
      }

      void setName(string name){
        this->name = name; 
      }
      void setType(string type){
        this->type = type; 
      }
      void setNextSymbol(SymbolInfo* nextSymbol){
        this->nextSymbol = nextSymbol;
      }
      void print(){
        cout<<name<<" "<<type<<endl; 
      }
}; 
