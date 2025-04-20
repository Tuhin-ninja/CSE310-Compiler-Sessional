#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include "hash_functions.hpp"

const int BUCKET_SIZE = 100;

int main(int argc, char* argv[]) {
    std::string hashName = "sdbm";
    std::string inputFile = "input.txt";
    std::string outputFile = "report.csv";

    // Parse args
    if (argc >= 2) hashName = argv[1];
    if (argc >= 3) inputFile = argv[2];
    if (argc >= 4) outputFile = argv[3];

    HashFunction hashFunc = getHashFunctionByName(hashName);
    std::vector<int> buckets(BUCKET_SIZE, 0);
    std::ifstream in(inputFile);
    std::string word;

    while (in >> word) {
        unsigned int hashValue = hashFunc(word);
        int index = hashValue % BUCKET_SIZE;
        buckets[index]++;
    }

    int collisions = 0;
    for (int count : buckets) {
        if (count > 1) collisions += (count - 1);
    }

    double quality = static_cast<double>(collisions) / BUCKET_SIZE;

    std::ofstream out(outputFile);
    out << "Hash Function,Collisions,Bucket Size,Collision Ratio\n";
    out << hashName << "," << collisions << "," << BUCKET_SIZE << "," << quality << "\n";

    std::cout << "Report written to " << outputFile << "\n";
}
