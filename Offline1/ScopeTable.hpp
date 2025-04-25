#include "SymbolInfo.hpp"
#include "OutputManager.hpp"
#include "hash_functions.hpp"



class ScopeTable
{
    // This class implements the hash table --> The symbols in the same scope are arranged
    // in the bucketList based on the appropriate hash function. For that reason we need a
    // bucketList and to communicate between different ScopeTables i.e. we need to store
    // the parentScope Table also. we will follow a  tree structure.
private:
    SymbolInfo **scopeVariables;
    ScopeTable *parentScope;
    int size;
    string id;
    ofstream &logout;
    HashFunction hashFunction;
    int collisionCount;

public:
    ScopeTable(int size, string id, ofstream &outputFile, ScopeTable *parentScope = nullptr, string hashFunctionName = "sdbm") : logout(outputFile)
    {
        this->size = size;
        this->id = id;
        this->parentScope = parentScope;
        this->hashFunction = getHashFunctionByName(hashFunctionName);
        scopeVariables = new SymbolInfo *[size];
        this->collisionCount = 0;

        for (int i = 0; i < size; i++)
        {
            scopeVariables[i] = nullptr;
        }
    }

    bool insert(SymbolInfo *symbolInfo)
    {

        long long index = hashFunction(symbolInfo->getName(), this->size) % this->size;
        if (index < 0)
            index *= -1;
        SymbolInfo *temp = scopeVariables[index];
        collisionCount = 0;
        SymbolInfo *prev = nullptr;
        int count = 0;
        if (temp == nullptr)
        {
            scopeVariables[index] = symbolInfo;
            count++;
            logout << "\tInserted in ScopeTable# " << id << " at position " << index + 1 << ", " << count << endl;
            return true;
        }
        else
        {
            while (temp)
            {

                count++;
                if (temp->getName() == symbolInfo->getName())
                {
                    string symbol = symbolInfo->getName(); 
                    symbol = "\'" + symbol + "\'";
                    logout << "\t" << symbol << " already exists in the current ScopeTable" << endl;
                    return false;
                }
                prev = temp;
                temp = temp->getNextSymbol();
            }
            collisionCount++;
            count++;
            prev->setNextSymbol(symbolInfo);
            logout << "\tInserted in ScopeTable# " << id << " at position " << index + 1 << ", " << count << endl;
            return true;
        }
    }


    int getCollisionCount()
    {

        return collisionCount;
    }   

    SymbolInfo *lookUp(string symbol)
    {
        long long index = hashFunction(symbol, this->size) % this->size;
        if (index < 0)
            index *= -1;
        SymbolInfo *temp = scopeVariables[index];
        int count = 0;
        while (temp)
        {
            count++;
            if (temp->getName() == symbol)
            {
                symbol = "\'" + symbol + "\'";
                logout << "\t" << symbol << " found in ScopeTable# " << id << " at position " << index + 1 << ", " << count << endl;
                return temp;
            }
            temp = temp->getNextSymbol();
        }

        return nullptr;
    }

    bool Delete(string symbol)
    {
        long long index = hashFunction(symbol, this->size) % this->size;
        if (index < 0)
            index *= -1;
        SymbolInfo *temp = scopeVariables[index];
        SymbolInfo *prev = nullptr;
        int count = 0;
        while (temp)
        {
            count++;
            if (temp->getName() == symbol)
            {
                if (prev == nullptr)
                {
                    scopeVariables[index] = temp->getNextSymbol();
                }
                else
                {
                    prev->setNextSymbol(temp->getNextSymbol());
                }
                delete temp;
                symbol = "\'" + symbol + "\'";
                logout << "\tDeleted " << symbol << " from ScopeTable# " << id << " at position " << index + 1 << ", " << count << endl;
                return true;
            }

            prev = temp;
            temp = temp->getNextSymbol();
        }

        logout << "\tNot found in the current ScopeTable" << endl;
        return false;
    }

    void printScopeTable(int tab = 0)
    {
        int tabs = tab;
        while (tabs--)
        {
            logout << "\t";
        }
        logout << "\tScopeTable# " << id << endl;
        for (int i = 0; i < this->size; i++)
        {
            tabs = tab;
            while (tabs--)
            {
                logout << "\t";
            }
            logout << "\t" << i + 1 << "-->" << " ";
            printSingleList(i);
            logout << endl;
        }
    }

    void printSingleList(int index)
    {
        SymbolInfo *temp = scopeVariables[index];
        while (temp)
        {
            logout << temp->to_string() << " ";
            temp = temp->getNextSymbol();
        }
    }

    void setID(string id)
    {
        this->id = id;
    }

    string getID()
    {
        return this->id;
    }

    void setParentScope(ScopeTable *parentScope)
    {
        this->parentScope = parentScope;
    }

    ScopeTable *getParentScope()
    {
        return parentScope;
    }

    ~ScopeTable() {
        for (int i = 0; i < size; i++) {
            SymbolInfo *temp = scopeVariables[i];
            while (temp) {
                SymbolInfo *toDelete = temp;
                temp = temp->getNextSymbol();
                delete toDelete;
            }
        }
        delete[] scopeVariables;
    }
};
