#include "2105002_SymbolInfo.hpp"
#include "2105002_hash_functions.hpp"

class ScopeTable
{
    // This class implements the hash table --> The symbols in the same scope are arranged
    // in the bucketList based on the appropriate hash function. For that reason we need a
    // bucketList and to communicate between different ScopeTables i.e. we need to store
    // the parentScope Table also. we will follow a  tree structure.
public:
    SymbolInfo **scopeVariables;
    ScopeTable *parentScope;
    ScopeTable *childScope;
    int size;
    int childNum; 
    int myNum; 
    string id;
    HashFunction hashFunction;
    int collisionCount;
    
public:
    string running ="";
    ScopeTable(int size, string id, ScopeTable *parentScope = nullptr, string hashFunctionName = "sdbm")
    {
        this->size = size;
        this->parentScope = parentScope;
        this->childNum = 0; 
        this->id = id;
        if (parentScope != nullptr) {
            parentScope->childScope = this;
        } 
        this->hashFunction = getHashFunctionByName(hashFunctionName);
        scopeVariables = new SymbolInfo *[size];
        this->collisionCount = 0;

        for (int i = 0; i < size; i++)
        {
            scopeVariables[i] = nullptr;
        }
    }

    string getFullID() {
        if (parentScope == nullptr) {
            return id;
        }
        return parentScope->getFullID() + "." + id;
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
                    running = "already exists in ScopeTable# " + this->getFullID() + " at position " + to_string(index) + ", " + to_string(count-1);
                    return false;
                }
                prev = temp;
                temp = temp->getNextSymbol();
            }
            collisionCount++;
            count++;
            prev->setNextSymbol(symbolInfo);
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
                return true;
            }
            prev = temp;
            temp = temp->getNextSymbol();
        }
        return false;
    }

    void printScopeTable(int tab = 0)
    {
        int tabs = tab;
        while (tabs--)
        {
            cout << "\t";
        }
        cout << "\tScopeTable# " << getFullID() << endl;
        for (int i = 0; i < this->size; i++)
        {
            tabs = tab;
            while (tabs--)
            {
                cout << "\t";
            }
            cout << "\t" << i + 1 << "-->" << " ";
            printSingleList(i);
            cout << endl;
        }
    }

    void printSingleList(int index)
    {
        SymbolInfo *temp = scopeVariables[index];
        while (temp)
        {
            cout << temp->to_string() << " ";
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
