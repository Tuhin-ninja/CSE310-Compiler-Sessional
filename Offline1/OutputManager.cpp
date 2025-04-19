#include "OutputManager.hpp"
#include <iostream>

std::ofstream logout;

// Initialize the stream
void initializeLogout(const std::string& filename) {
    logout.open(filename);
    if (!logout.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
} 