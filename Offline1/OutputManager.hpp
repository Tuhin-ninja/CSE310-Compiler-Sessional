#ifndef OUTPUT_MANAGER_H
#define OUTPUT_MANAGER_H

#include <fstream>
#include <string>

extern std::ofstream logout;
void initializeLogout(const std::string& filename);

#endif
