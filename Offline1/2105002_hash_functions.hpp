#pragma once
#include <string>
#include <functional>

// Define the hash function type
typedef std::function<unsigned int(const std::string&, unsigned int)> HashFunction;

// All these must match the typedef exactly!
unsigned int SDBMHash(const std::string& str, unsigned int num_buckets);
unsigned int hashFNV1a(const std::string& str, unsigned int num_buckets);
unsigned int hashMurmurInspired(const std::string& str, unsigned int num_buckets);

// Return correct hash function
HashFunction getHashFunctionByName(std::string name);
std::string getHashFunctionName(const std::string& name);
