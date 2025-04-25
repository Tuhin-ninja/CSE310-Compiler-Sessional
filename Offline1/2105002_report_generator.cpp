#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring> // For strcpy if needed

struct ReportEntry {
    char hashFunctionName[20];
    int totalCollisions;
    double collisionRatio;
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
