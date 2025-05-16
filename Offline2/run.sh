#!/bin/bash 


flex 2105002.l
mv lex.yy.c lex.yy.cpp
g++ -std=c++17 lex.yy.cpp 2105002_hash_functions.cpp SymbolTableHelper.cpp -o compiler

for i in 1 2 3
do
    log_file="io/input${i}_log.txt"
    token_file="io/input${i}_token.txt"
    ./compiler "io/input${i}.txt"

    tr -d '\r' < "$log_file" |
    sed 's/[[:space:]]*$//' |
    perl -0777 -pe 's/<CONST_CHAR,[\s]*>/<CONST_CHAR, >/g' > "temp_log_actual.txt"

    tr -d '\r' < "2105002_log.txt" |
    sed 's/[[:space:]]*$//' |
    perl -0777 -pe 's/<CONST_CHAR,[\s]*>/<CONST_CHAR, >/g' > "temp_log_expected.txt"

    if diff -q "temp_log_actual.txt" "temp_log_expected.txt" > /dev/null; then
        echo "Log Files are identical for input${i}.txt"
    else
        echo "Log Files differ for input${i}.txt"
        diff "temp_log_actual.txt" "temp_log_expected.txt"
        break
    fi

    tr -d '\r' < "$token_file" | sed 's/[[:space:]]*$//' > "temp_token_actual.txt"
    tr -d '\r' < "2105002_token.txt" | sed 's/[[:space:]]*$//' > "temp_token_expected.txt"

    if diff -q "temp_token_actual.txt" "temp_token_expected.txt" > /dev/null; then 
        echo "Token Files are identical for input${i}.txt"
    else
        echo "Token Files differ for input${i}.txt"
        diff "temp_token_actual.txt" "temp_token_expected.txt"
    fi

    echo "-------------------------------"
    echo "-------------------------------"
done

rm -f temp_log_actual.txt temp_log_expected.txt temp_token_actual.txt temp_token_expected.txt
