%{
#include <stdio.h>
#include <string.h>
#include "SymbolTableHelper.hpp"

int line_no = 1;  // Consistent variable name
int error_count = 0;  // Track total errors
FILE *token_file = NULL, *log_file = NULL;

char *processString(const char *str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    if (len < 2) return strdup("");  // Handle empty or malformed strings

    char *result = (char *)malloc(len + 1);  // Allocate enough space
    if (!result) {
        fprintf(log_file, "ERROR: Memory allocation failed in processString\n");
        return NULL;
    }

    int j = 0;
    int i;
    for (i = 1; i < len - 1; i++) {  // Start after opening quote, end before closing quote
        if (str[i] == '\\') {
            i++;
            if (i < len && str[i] == '\n') {
                continue;
            }
            if (i < len) {
                switch (str[i]) {
                    case 'n': result[j++] = '\n'; break;
                    case 't': result[j++] = '\t'; break;
                    case '\\': result[j++] = '\\'; break;
                    case '"': result[j++] = '"'; break;
                    case 'a': result[j++] = '\a'; break;
                    case 'f': result[j++] = '\f'; break;
                    case 'r': result[j++] = '\r'; break;
                    case 'b': result[j++] = '\b'; break;
                    case 'v': result[j++] = '\v'; break;
                    case '0': result[j++] = '\0'; break;
                    default: result[j++] = str[i];
                }
            }
        } else {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';
    return result;
}

void log_token(const char *token, const char *lexeme) {
    if (!log_file) return;
    char *result = processString(lexeme);

    if (strcmp(token, "<STRING>") == 0) {
        fprintf(log_file, "Line no %d: Token %s Lexeme %s found --> <STRING, %s>\n\n", line_no, token, lexeme, result);
    } else {
        fprintf(log_file, "Line no %d: Token %s Lexeme %s found\n\n", line_no, token, lexeme);
    }
}

void log_comment(const char *lexeme) {
    if (!log_file) return;

    // Make a copy of the lexeme
    char cleaned_lexeme[4096];  // Make sure it's large enough
    strncpy(cleaned_lexeme, lexeme, sizeof(cleaned_lexeme) - 1);
    cleaned_lexeme[sizeof(cleaned_lexeme) - 1] = '\0';

    // Strip trailing newlines
    size_t len = strlen(cleaned_lexeme);
    while (len > 0 && (cleaned_lexeme[len - 1] == '\n' || cleaned_lexeme[len - 1] == '\r')) {
        cleaned_lexeme[--len] = '\0';
    }

    // Now print on same line
    fprintf(log_file, "Line no %d: Token <COMMENT> Lexeme %s found\n\n", line_no, cleaned_lexeme);
}

void report_error(const char *msg, const char *yy) {
    if (!log_file) return;
    fprintf(log_file, "Error at line no %d: %s %s\n\n", line_no, msg,yy);
    error_count++;  // Increment error count
}

// Convert escape sequences for characters and strings
char convertEscapeChar(const char *lexeme) {
    if (lexeme[1] == '\\') {
        switch (lexeme[2]) {
            case 'n': return '\n';
            case 't': return '\t';
            case '\\': return '\\';
            case '\'': return '\'';
            case 'a': return '\a';
            case 'f': return '\f';
            case 'r': return '\r';
            case 'b': return '\b';
            case 'v': return '\v';
            case '0': return '\0';
            default: return lexeme[2];
        }
    }
    return lexeme[1];
}


// Helper function to convert E to e in floating-point lexemes
char *convertFloatLexeme(const char *lexeme) {
    if (!lexeme) return NULL;
    char *result = strdup(lexeme);
    if (!result) {
        fprintf(log_file, "ERROR: Memory allocation failed in convertFloatLexeme\n");
        return NULL;
    }
    for (int i = 0; result[i]; i++) {
        if (result[i] == 'E') {
            
        }
    }
    return result;
}

%}


%option noyywrap

WHITESPACE [ \t\r\v\f]+
UNTERMINATED_SINGLE_QUOTE \'(\n|$)
NEWLINE \n
DIGIT [0-9]
LETTER [a-zA-Z_]
ALNUM [a-zA-Z_0-9]
IDENTIFIER {LETTER}{ALNUM}*
FLOAT ({DIGIT}+\.{DIGIT}*([Ee][+-]?{DIGIT}+)?)|(\.{DIGIT}+([Ee][+-]?{DIGIT}+)?)|({DIGIT}+[Ee][+-]?{DIGIT}+)
INTEGER {DIGIT}+
UNFINISHED_STRING (\")([^\\\n\"]*(\\(({NEWLINE})|({ALNUM}|[\\\"\'])))*)*
SINGLE_COMMENT "//"[^\n\\]*(\\[\n][^\n\\]*)*(\\)?(\n)?
MULTI_COMMENT "/*"([^*]|[*][^*/\n]|[\\][\n])*[*]+"/"
MULTI_DECIMAL {DIGIT}+(?:\.{DIGIT}*){2,}
ILL_NUMBER {DIGIT}+[Ee][+-]?{DIGIT}*\.{DIGIT}*|{DIGIT}+[Ee][+-]?{DIGIT}*\.[Ee]
INVALID_NUM_SUFFIX {INTEGER}([a-dA-Df-zF-Z_]+|[a-dA-Df-zF-Z_]{ALNUM}*)
UNFINISHED_CHAR \'([^\\'\n]|\\.)?(\n|$)
CHAR   '[^'\\\a\n\r\b\v\t\f\0]'
MULTI_CHAR \'([^'\n\\]|\\.){2,}\'
UNFINISHED_COMMENT "/*"([^*]|[*][^*/\n]|[\\][\n])*
UNRECOGNIZED .

%%

{NEWLINE} { 
    line_no++;
 }

{WHITESPACE} { /* Ignore whitespace */ }

"if"      { log_token("<IF>", yytext); fprintf(token_file, "<IF> "); }
"else"    { log_token("<ELSE>", yytext); fprintf(token_file, "<ELSE> "); }
"goto"    { log_token("<GOTO>", yytext); fprintf(token_file, "<GOTO> "); }
"for"     { log_token("<FOR>", yytext); fprintf(token_file, "<FOR> "); }
"while"   { log_token("<WHILE>", yytext); fprintf(token_file, "<WHILE> "); }
"long"    { log_token("<LONG>", yytext); fprintf(token_file, "<LONG> "); }
"do"      { log_token("<DO>", yytext); fprintf(token_file, "<DO> "); }
"break"   { log_token("<BREAK>", yytext); fprintf(token_file, "<BREAK> "); }
"short"   { log_token("<SHORT>", yytext); fprintf(token_file, "<SHORT> "); }
"int"     { log_token("<INT>", yytext); fprintf(token_file, "<INT> "); }
"char"    { log_token("<CHAR>", yytext); fprintf(token_file, "<CHAR> "); }
"static"  { log_token("<STATIC>", yytext); fprintf(token_file, "<STATIC> "); }
"float"   { log_token("<FLOAT>", yytext); fprintf(token_file, "<FLOAT> "); }
"double"  { log_token("<DOUBLE>", yytext); fprintf(token_file, "<DOUBLE> "); }
"unsigned" { log_token("<UNSIGNED>", yytext); fprintf(token_file, "<UNSIGNED> "); }
"void"    { log_token("<VOID>", yytext); fprintf(token_file, "<VOID> "); }
"return"  { log_token("<RETURN>", yytext); fprintf(token_file, "<RETURN> "); }
"switch"  { log_token("<SWITCH>", yytext); fprintf(token_file, "<SWITCH> "); }
"case"    { log_token("<CASE>", yytext); fprintf(token_file, "<CASE> "); }
"default" { log_token("<DEFAULT>", yytext); fprintf(token_file, "<DEFAULT> "); }
"continue" { log_token("<CONTINUE>", yytext); fprintf(token_file, "<CONTINUE> "); }


{UNTERMINATED_SINGLE_QUOTE} {
    report_error("Unterminated character", yytext);
    line_no++;
}


{FLOAT} {
    char *modified_lexeme = convertFloatLexeme(yytext);
    if (!modified_lexeme) return 1;
    log_token("<CONST_FLOAT>", modified_lexeme);
    fprintf(token_file, "<CONST_FLOAT, %s> ", modified_lexeme);
    insert_symbol(log_file,modified_lexeme, "CONST_FLOAT");
    free(modified_lexeme);
}

{INTEGER} {
    log_token("<CONST_INT>", yytext);
    fprintf(token_file, "<CONST_INT, %s> ", yytext);
    insert_symbol(log_file,yytext, "CONST_INT");
}

{UNFINISHED_CHAR} {
    report_error("Unterminated character",yytext);
    line_no++;
}

'' { 
    fprintf(log_file, "Error at line no %d: Empty character constant error %s\n\n\n", line_no,yytext);  
    error_count++;
 }


'[^'\\][^'\n]+' { 
    fprintf(log_file, "Error at line no %d: Multi character constant error %s\n\n\n", line_no,yytext);
    error_count++;
}

'\\[^'nt\\afbrv0]' { report_error("Unrecognized escape sequence in character constant", yytext); }

'\\[^'nt\\afbrv0] { report_error("Unterminated character", yytext); 
    line_no++;
}

'[^'\n]*$ { report_error("Unterminated character", yytext); }




'\\\\' { 
    char c = convertEscapeChar(yytext);
    char str[2] = {c, '\0'};
    log_token("<CONST_CHAR>", str);
    fprintf(token_file, "<CONST_CHAR, %s> ", str);
    insert_symbol(log_file,str, "CONST_CHAR");
}
'\\\'' { 
    char c = convertEscapeChar(yytext);
    char str[2] = {c, '\0'};
    log_token("<CONST_CHAR>", str);
    fprintf(token_file, "<CONST_CHAR, %s> ", str);
    insert_symbol(log_file,str, "CONST_CHAR");
}
'\\n' { 
    char c = convertEscapeChar(yytext);
    char str[2] = {c, '\0'};
    fprintf(log_file, "Line no %d: Token <CONST_CHAR> Lexeme %s found --> <CONST_CHAR, %s>\n\n", line_no, yytext, str);
    fprintf(token_file, "<CONST_CHAR, %s> ", str);
    insert_symbol(log_file,yytext, "CONST_CHAR");
}


'\\r' { 
    char c = convertEscapeChar(yytext);
    char str[2] = {c, '\0'};
    fprintf(log_file, "Line no %d: Token <CONST_CHAR> Lexeme %s found --> <CONST_CHAR, %s>\n\n", line_no, yytext, str);
    fprintf(token_file, "<CONST_CHAR, %s> ", str);
    insert_symbol(log_file,yytext, "CONST_CHAR");
}
'\\t' {
    char c = convertEscapeChar(yytext);
    char str[2] = {c, '\0'};
    fprintf(log_file, "Line no %d: Token <CONST_CHAR> Lexeme %s found --> <CONST_CHAR, %s>\n\n", line_no, yytext, str);
    fprintf(token_file, "<CONST_CHAR, %s> ", str);
    insert_symbol(log_file,yytext, "CONST_CHAR");
}

'\\a' {
    char c = convertEscapeChar(yytext);
    char str[2] = {c, '\0'};
    fprintf(log_file, "Line no %d: Token <CONST_CHAR> Lexeme %s found --> <CONST_CHAR, %s>\n\n", line_no, yytext, str);
    fprintf(token_file, "<CONST_CHAR, %s> ", str);
    insert_symbol(log_file,yytext, "CONST_CHAR");
}
'\\f' {
    char c = convertEscapeChar(yytext);
    char str[2] = {c, '\0'};
    fprintf(log_file, "Line no %d: Token <CONST_CHAR> Lexeme %s found --> <CONST_CHAR, %s>\n\n", line_no, yytext, str);
    fprintf(token_file, "<CONST_CHAR, %s> ", str);
    insert_symbol(log_file,yytext, "CONST_CHAR");
}
'\\b' {
    char c = convertEscapeChar(yytext);
    char str[2] = {c, '\0'};
    fprintf(log_file, "Line no %d: Token <CONST_CHAR> Lexeme %s found --> <CONST_CHAR, %s>\n\n", line_no, yytext, str);
    fprintf(token_file, "<CONST_CHAR, %s> ", str);
    insert_symbol(log_file,yytext, "CONST_CHAR");
}

'\\0' {
    char c = convertEscapeChar(yytext);
    char str[2] = {c, '\0'};
    fprintf(log_file, "Line no %d: Token <CONST_CHAR> Lexeme %s found --> <CONST_CHAR, %s>\n\n", line_no, yytext, str);
    fprintf(token_file, "<CONST_CHAR, %s> ", str);
    insert_symbol(log_file,yytext, "CONST_CHAR");
}

'\\v' {
    char c = convertEscapeChar(yytext);
    char str[2] = {c, '\0'};
    fprintf(log_file, "Line no %d: Token <CONST_CHAR> Lexeme %s found --> <CONST_CHAR, %s>\n\n", line_no, yytext, str);
    fprintf(token_file, "<CONST_CHAR, %s> ", str);
    insert_symbol(log_file,yytext, "CONST_CHAR");
}

\'([^\\'\n]|\\[nt\\'\"0abfrv])\' { 
    char c = convertEscapeChar(yytext);
    char str[2] = {c, '\0'};
    fprintf(log_file, "Line no %d: Token <CONST_CHAR> Lexeme %s found --> <CONST_CHAR, %s>\n\n", line_no, yytext, str);
    fprintf(token_file, "<CONST_CHAR, %s> ", str);
    insert_symbol(log_file,yytext, "CONST_CHAR");
}

(\")([^\\\n\"]*(\\(({NEWLINE})|({ALNUM})|[\\\"\']))*)*(\") {
    char *s2 = yytext;
    int countnewline = 0;
    while(*s2!='\0'){
        if(*s2 == '\n'){
            line_no++;
        }
        s2++;
    }
    
    int fl=0;
    char *s = yytext;
    char *ans = new char[10000];
    char *res = ans;
    string temp = yytext;
    for(int i=0;i<temp.length();i++){
        while(*s){
            if(*s == '\\'){
                if(*(s+1) == 'n'){
                    s++;
                    *ans = '\n';
                }
                else if(*(s+1) == 't'){
                    s++;
                    *ans = '\t';
                }
                else if(*(s+1) == 'a'){
                    s++;
                    *ans = '\a';
                }
                else if(*(s+1) == 'f'){
                    s++;
                    *ans = '\f';
                }
                else if(*(s+1) == 'b'){
                    s++;
                    *ans = '\b';
                }
                else if(*(s+1) == 'v'){
                    s++;
                    *ans = '\v';
                }
                else if(*(s+1) == 'r'){
                    s++;
                    *ans = '\r';
                }
                else if(*(s+1) == '\"'){
                    s++;
                    *ans = '\"';
                }
                else if(*(s+1) == '\\'){
                    s++;
                    *ans = '\\';
                }
                else if(*(s+1) == '\''){
                    s++;
                    *ans = '\'';
                }
                else if(*(s+1) == '\n'){
                    ans--;
                }
                else if(*(s+1) == '\r'){
                    ans--;
                }
                else{
                    fl++;
                }

            }
            else{
                if(*s == 10 || *s == 13){
                    
                    ans--;
                    if(*ans == '\r'){
                        ans--;
                    }
                }
                else{
                    
                    *ans = *s;
                }
            }
            ans++;
            s++;    
        }
    }
    *ans = '\0';
    if(fl!=0){
        
        report_error("Unrecognized error",yytext);
        error_count++;
    }
    else{
        temp = res;
        log_token("<STRING>", yytext);
        char *result = processString(yytext);
        fprintf(token_file,"<STRING, %s> ", result);

    }

}

"+"|"-" {
    log_token("<ADDOP>", yytext);
    fprintf(token_file, "<ADDOP, %s> ", yytext);
}

"*"|"/"|"%" {
    log_token("<MULOP>", yytext);
    fprintf(token_file, "<MULOP, %s> ", yytext);
}

"++"|"--" {
    log_token("<INCOP>", yytext);
    fprintf(token_file, "<INCOP, %s> ", yytext);
}

"<"|"<="|">"|">="|"=="|"!=" {
    log_token("<RELOP>", yytext);
    fprintf(token_file, "<RELOP, %s> ", yytext);
}

"=" {
    log_token("<ASSIGNOP>", yytext);
    fprintf(token_file, "<ASSIGNOP, %s> ", yytext);
}

"&&"|"||" {
    log_token("<LOGICOP>", yytext);
    fprintf(token_file, "<LOGICOP, %s> ", yytext);
}

"!" {
    log_token("<NOT>", yytext);
    fprintf(token_file, "<NOT, %s> ", yytext);
}

"(" {
    log_token("<LPAREN>", yytext);
    fprintf(token_file, "<LPAREN, %s> ", yytext);
}

")" {
    log_token("<RPAREN>", yytext);
    fprintf(token_file, "<RPAREN, %s> ", yytext);
}

"{" {
    log_token("<LCURL>", yytext);
    fprintf(token_file, "<LCURL, %s> ", yytext);
    enter_scope();
}

"}" {
    log_token("<RCURL>", yytext);
    fprintf(token_file, "<RCURL, %s> ", yytext);
    exit_scope();
}

"[" {
    log_token("<LTHIRD>", yytext);
    fprintf(token_file, "<LTHIRD, %s> ", yytext);
}

"]" {
    log_token("<RTHIRD>", yytext);
    fprintf(token_file, "<RTHIRD, %s> ", yytext);
}

"," {
    log_token("<COMMA>", yytext);
    fprintf(token_file, "<COMMA, %s> ", yytext);
}

";" {
    log_token("<SEMICOLON>", yytext);
    fprintf(token_file, "<SEMICOLON, %s> ", yytext);
}


{IDENTIFIER} {
    log_token("<ID>", yytext);
    fprintf(token_file, "<ID, %s> ", yytext);
    insert_symbol(log_file,yytext, "ID");
}

{SINGLE_COMMENT} {
    int count = 0;
     for (int i = 0; yytext[i]; i++) {
        if (yytext[i] == '\n'){
            line_no++;
            count++;
        }
    }

        line_no--;
        log_comment(yytext);
        line_no++;

}

{MULTI_COMMENT} {
    for (int i = 0; yytext[i]; i++) {
        if (yytext[i] == '\n') line_no++;
    }
    log_comment(yytext);
}

{MULTI_DECIMAL} {
    fprintf(log_file, "Error at line no %d: Too many decimal points %s\n\n\n", line_no,yytext);
    error_count++;  
}

{ILL_NUMBER} {
    fprintf(log_file, "Error at line no %d: Ill formed number %s\n\n\n", line_no,yytext);
    error_count++;
}

{INVALID_NUM_SUFFIX} {
    report_error("Invalid prefix on ID or invalid suffix on Number", yytext);
}

{UNFINISHED_STRING} {
   int len = strlen(yytext);
   int count = 0;
    for (int i = 0; i < len-1; i++) {
       if (yytext[i] == '\n') {
          count++;
        }
    }
    report_error("Unterminated string", yytext);
    line_no += count;
}

{UNFINISHED_COMMENT} {
   int len = strlen(yytext);
   int count = 1;
    for (int i = 0; i < len-1; i++) {
       if (yytext[i] == '\n') {
          count++;
        }
    }
    report_error("Unterminated comment", yytext);
    line_no += count;
}

{UNRECOGNIZED} {
    report_error("Unrecognized character", yytext);
}

<<EOF>> {
    if (yytext && *yytext) {
        fprintf(log_file, "DEBUG: Forcing reprocessing of remaining input\n");
        yymore();  // Append remaining input to the current match
    }
    return 0;
}

%%

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Please provide input file\n");
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        printf("Cannot open input file\n");
        return 1;
    }

    log_file = fopen("2105002_log.txt", "w");
    if (!log_file) {
        printf("Cannot open log file\n");
        fclose(input_file);
        return 1;
    }

    token_file = fopen("2105002_token.txt", "w");
    if (!token_file) {
        printf("Cannot open token file\n");
        fclose(input_file);
        fclose(log_file);
        return 1;
    }

    init_symbol_table();
    yyin = input_file;
    yylex();

    print_symbol_table(log_file);
    fprintf(log_file, "Total lines: %d\n", line_no);
    fprintf(log_file, "Total errors: %d\n", error_count);

    fclose(input_file);
    fclose(log_file);
    fclose(token_file);

    return 0;
}