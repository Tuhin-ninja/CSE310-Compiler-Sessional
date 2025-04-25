#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <sstream>
#include <string>
#include "2105002_SymbolTable.hpp"
#include "2105002_OutputManager.cpp"

using namespace std;

struct ReportEntry {
    char hashFunctionName[20];
    int totalCollisions;
    double collisionRatio;
    string inputTakenFrom;
};

void generateReportCSV(ReportEntry entries[], int entryCount, const char* filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Cannot open file for writing: " << filename << std::endl;
        return;
    }

    outFile << "Hash Function,Total Collisions,Collision Ratio\n";

    for (int i = 0; i < entryCount; ++i) {
        outFile << entries[i].hashFunctionName << ","
                << entries[i].totalCollisions << ","
                << std::fixed << std::setprecision(6) << entries[i].collisionRatio << "\n";
    }

    outFile.close();
}

void appendReportEntry(const std::string& filename, const ReportEntry& entry) {
    std::ifstream checkFile(filename);
    bool isEmpty = checkFile.peek() == std::ifstream::traits_type::eof();
    checkFile.close();

    std::ofstream file(filename, std::ios::app); // Append mode
    if (!file.is_open()) {
        std::cerr << "Error opening report file for writing.\n";
        return;
    }

    if (isEmpty) {
        file << "Hash Function,Total Collisions,Collision Ratio,Input Taken From\n";
    }

    file << entry.hashFunctionName << ","
         << entry.totalCollisions << ","
         << std::fixed << std::setprecision(6) << entry.collisionRatio << ","
         <<entry.inputTakenFrom << "\n";
    
    file.close();
}




class Compiler
{
private:
    ifstream in;
    SymbolTable *symbolTable;
    int commandCount;
    string hashFunctionName;
    int collisionCount;
    double collisionRatio;
    int insertionCount;

public:
    Compiler(string inputFile, string outputFile, string hashFunctionName)
    {
        this->hashFunctionName = hashFunctionName;
        in.open(inputFile);
        initializeLogout(outputFile);
        this->commandCount = 0;
        this->collisionCount = 0;
        this->collisionRatio = 0.0;
        this->insertionCount = 0;
        if (!in.is_open())
        {
            cout << "Failed to open inputFile" << endl;
        }
        else if (!logout.is_open())
        {
            cout << "Failed to open outputFile" << endl;
        }
        else
        {
            string line;
            getline(this->in, line);

            symbolTable = new SymbolTable(stoi(line), logout, hashFunctionName);
            logout << "\tScopeTable# " << symbolTable->getCurrentScope()->getID() << " created" << endl;
        }
    }

    void Insertion(string line)
    {

        stringstream ss(line);
        char delimeter = ' ';
        string word;
        int count = 0;
        string words[100]; // assuming max words 100. if found better solution, this should be modified...
        while (getline(ss, word, delimeter))
        {
            words[count++] = word;
        }

        // cout<<"Count is : "<<count<<endl;
        if (count < 2)
        {
            logout << "\tNumber of parameters mismatch for the command I" << endl;
            return;
        }
        string wordType = words[2];
        string name = words[1];
        string type;

        if (wordType == "FUNCTION")
        {
            stringstream ss;
            for (int i = 4; i < count; ++i)
            {
                ss << words[i];
                if (i != count - 1)
                    ss << ",";
            }

            type = words[2] + "," + words[3] + "<==" + "(" + ss.str() + ")";
            // cout<<type<<endl;
        }

        else if (wordType == "STRUCT" || wordType == "UNION")
        {
            stringstream ss;
            for (int i = 3; i < count; i += 2)
            {
                ss << "(" + words[i] + "," + words[i + 1] << ")";
                if (i != count - 2)
                    ss << ",";
            }

            type = wordType + "," + "{" + ss.str() + "}";
        }

        else if (wordType != " ")
        {
            if (count != 3)
            {
                // cout<<"in "<<count<<endl;
                logout << "\tNumber of parameters mismatch for the command I" << endl;
                return;
            }
            type = words[2];
        }

        SymbolInfo *symbolInfo = new SymbolInfo(name, type);
        // cout<<symbolInfo->to_string()<<endl;
        symbolTable->Insert(symbolInfo);
        this->insertionCount++;
    }

    void Lookup(string line)
    {
        stringstream ss(line);
        char delimeter = ' ';
        string word;
        int count = 0;
        string words[100]; // assuming max words 100. if found better solution, this should be modified...
        while (getline(ss, word, delimeter))
        {
            words[count++] = word;
        }
        string name = words[1];
        if (count != 2)
        {
            logout << "\tNumber of parameters mismatch for the command L" << endl;
            return;
        }
        SymbolInfo *symbolInfo = symbolTable->LookUp(name);
        if (symbolInfo != nullptr)
        {
            // cout<<symbolInfo->to_string()<<endl;
        }
        else
        {
            // cout<<"Not found!"<<endl;
        }
    }

    void Delete(string line)
    {
        stringstream ss(line);
        char delimeter = ' ';
        string word;
        int count = 0;
        string words[100];
        while (getline(ss, word, delimeter))
        {
            words[count++] = word;
        }

        if (count != 2)
        {
            logout << "\tNumber of parameters mismatch for the command D" << endl;
            return;
        }

        string name = words[1];
        symbolTable->Remove(name);
    }

    void Print(string line)
    {
        stringstream ss(line);
        char delimeter = ' ';
        string word;
        int count = 0;
        string words[100];
        while (getline(ss, word, delimeter))
        {
            words[count++] = word;
        }

        if (count != 2)
        {
            logout << "\tNumber of parameters mismatch for the command I" << endl;
            return;
        }

        if (words[1] == "C")
        {
            symbolTable->printCurrentScope();
        }

        if (words[1] == "A")
        {
            symbolTable->printAllScopes();
        }
    }

    void trimTrailingSpaces(std::string &line)
    {
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch)
                                { return !std::isspace(ch); })
                       .base(),
                   line.end());
    }

    SymbolTable *getSymbolTable()
    {
        return this->symbolTable;
    }

    void compile()
    {
        string line;
        while (getline(in, line))
        {
            trimTrailingSpaces(line);
            commandCount++;
            logout << "Cmd " << commandCount << ": " << line << endl;
            switch (line[0])
            {
            case 'I':
                Insertion(line);
                break;
            case 'L':
                Lookup(line);
                break;
            case 'D':
                Delete(line);
                break;
            case 'P':
                Print(line);
                break;
            case 'S':
                symbolTable->EnterScope();
                break;
            case 'E':
                symbolTable->ExitScope();
                break;
            case 'Q':
                this->collisionCount = symbolTable->getCollisionCount();
                this->collisionRatio = symbolTable->getCollisionRatio();
                delete this->symbolTable;
                return;
            }
        }
    }

    int getCollisionCount()
    {
        return this->collisionCount;
    }
    double getCollisionRatio()
    {
        return this->collisionRatio;
    }

    ~Compiler()
    {
        in.close();
        logout.close();
        // cout<<"Compiler Deleted"<<endl;
    }
};

int main(int argc, char* argv[])
{
    string hashFunctionName = "sdbm"; //default hash function
    if (argc < 3 || argc > 4) {
        cerr << "Error: Invalid number of arguments." << endl;
        cerr << "Usage: " << argv[0] << " <input_file> <output_file> [<hash_function_name>]" << endl;
        return 1;
    }

    string inputFile = argv[1];
    string outputFile = argv[2];
    if (argc == 4) {
        hashFunctionName = argv[3];
    }
    const string reportFile = "report.csv"; 

    Compiler* compiler = new Compiler(inputFile, outputFile, hashFunctionName);
    compiler->compile();
    ReportEntry entry;
    strcpy(entry.hashFunctionName, hashFunctionName.c_str());
    entry.totalCollisions = compiler->getCollisionCount();
    entry.collisionRatio = compiler->getCollisionRatio();
    entry.inputTakenFrom = inputFile;
    delete compiler;
    appendReportEntry(reportFile, entry);
    return 0;
}