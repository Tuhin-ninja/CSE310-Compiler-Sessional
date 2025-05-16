#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <iostream>
#include <string>
#include "2105002_ScopeTable.hpp"

using namespace std;

class SymbolTable {
private:
    ScopeTable* currentScopeTable;
    int size;
    int scopeCounter;
    string hashFunctionName;
    int collisionCount;
    double collisionRatio;
    int insertionCount;

public:
    SymbolTable(int size, string hashFunctionName) {
        this->size = size;
        this->scopeCounter = 1;
        this->currentScopeTable = new ScopeTable(size, to_string(scopeCounter), nullptr, hashFunctionName);
        this->hashFunctionName = hashFunctionName;
        this->collisionCount = 0;
        this->collisionRatio = 0.0;
        this->insertionCount = 0;
    }

    bool Insert(SymbolInfo* symbolInfo) {
        bool result = currentScopeTable->insert(symbolInfo);
        if (result) insertionCount++;
        this->collisionCount += currentScopeTable->getCollisionCount();
        return result;
    }

    int getCollisionCount() {
        return this->collisionCount;
    }

    double getCollisionRatio() {
        return (double)collisionCount / (double)insertionCount;
    }

    bool Remove(string symbol) {
        return currentScopeTable->Delete(symbol);
    }

    SymbolInfo* LookUp(string symbol) {
        SymbolInfo* symbolInfo;
        ScopeTable* temp = currentScopeTable;
        while (temp) {
            symbolInfo = temp->lookUp(symbol);
            if (symbolInfo != nullptr) {
                return symbolInfo;
            }
            temp = temp->getParentScope();
        }
        return nullptr;
    }

    void EnterScope() {
        if (currentScopeTable == nullptr) {
            this->scopeCounter++;
            currentScopeTable = new ScopeTable(size, to_string(scopeCounter), nullptr, hashFunctionName);
        } else {
            currentScopeTable->childNum++;
            currentScopeTable = new ScopeTable(size, to_string(currentScopeTable->childNum), currentScopeTable, hashFunctionName);
        }
    }

    void ExitScope() {
        if (currentScopeTable->getParentScope() == nullptr) {
            return;
        }
        ScopeTable* temp = currentScopeTable;
        currentScopeTable = currentScopeTable->getParentScope();
        delete temp;
    }

    void printCurrentScope() {
        currentScopeTable->printScopeTable();
    }

    void printAllScopes() {
        ScopeTable* temp = currentScopeTable;
        int count = 0;
        while (temp != nullptr) {
            temp->printScopeTable(count);
            temp = temp->getParentScope();
            count++;
        }
    }

    ScopeTable* getCurrentScope() {
        return this->currentScopeTable;
    }

    ~SymbolTable() {
        ScopeTable* temp = currentScopeTable;
        while (temp != nullptr) {
            ScopeTable* parent = temp->getParentScope();
            delete temp;
            temp = parent;
        }
    }
};

#endif
