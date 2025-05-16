#include "SymbolTableHelper.hpp"
#include <fstream>

SymbolTable *symbolTable = nullptr;

void init_symbol_table()
{
    static std::ofstream logout("based.txt"); // dummy log for constructor
    symbolTable = new SymbolTable(7, "hash");
}

void insert_symbol(FILE *log_file, const char *name, const char *type)
{
    SymbolInfo *info = new SymbolInfo(name, type);
    string symbolName = info->to_string();
    bool result = symbolTable->Insert(info);
    if(!result){
        string running = symbolTable->getCurrentScope()->running;
        fprintf(log_file, "%s %s\n\n",symbolName.c_str() ,running.c_str());
        return;
    }
    print_symbol_table(log_file);
}

void print_symbol_table(FILE *log_file)
{
    ScopeTable *tempScope = symbolTable->getCurrentScope();
    while (tempScope != nullptr)
    {
        std::string ids = tempScope->getFullID();

        fprintf(log_file, "ScopeTable # %s\n", ids.c_str());
        // Print non-empty bucket list
        for (int i = 0; i < 7; i++)
        {
            SymbolInfo *scopeVariable = tempScope->scopeVariables[i];
            if (scopeVariable == nullptr)
            {
                continue;
            }
            std::string print = "" + std::to_string(i) + " --> ";
            fprintf(log_file, "%s", print.c_str());
            while (scopeVariable != nullptr)
            {
                std::string var = scopeVariable->to_string();
                fprintf(log_file, "%s", var.c_str());
                scopeVariable = scopeVariable->getNextSymbol();
            }
            fprintf(log_file, "\n");
        }
        tempScope = tempScope->getParentScope();
    }
    fprintf(log_file, "\n");
    // Extra newline for separation between scope tables
}

void exit_scope()
{
    symbolTable->ExitScope();
}

void enter_scope()
{
    symbolTable->EnterScope();
}
