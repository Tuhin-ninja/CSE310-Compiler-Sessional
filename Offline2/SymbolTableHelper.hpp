#ifndef SYMBOL_TABLE_HELPER_HPP
#define SYMBOL_TABLE_HELPER_HPP

#include "2105002_SymbolTable.hpp"

extern SymbolTable* symbolTable;

void init_symbol_table();
void insert_symbol(FILE *output_file, const char* name, const char* type);
void print_symbol_table(FILE *output_file);
void exit_scope();
void enter_scope();

#endif
