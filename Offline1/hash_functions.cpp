#include "hash_functions.hpp"




unsigned int SDBMHash(const std::string& str, unsigned int num_buckets) { unsigned int hash = 0;
    unsigned int len = str.length();
    for (unsigned int i = 0; i < len; i++)
    {
    hash = ((str[i]) + (hash << 6) + (hash << 16) - hash) %
    num_buckets; }
    return hash;
    }

unsigned int hashCustom1(const std::string& str, unsigned int num_buckets) {
    unsigned int hash = 5381;
    for (char ch : str) {
        hash = ((hash << 5) + hash) + ch; // hash * 33 + ch
    }
    return hash % num_buckets;
}

unsigned int hashCustom2(const std::string& str, unsigned int num_buckets) {
    unsigned int hash = 0;
    for (char ch : str) {
        hash = hash * 101 + ch;
    }
    return hash % num_buckets;
}

HashFunction getHashFunctionByName(std::string name) {
    if (name == "sdbm") return SDBMHash;
    if (name == "custom1") return hashCustom1;
    if (name == "custom2") return hashCustom2;
    return SDBMHash; // default
}


std::string getHashFunctionName(const std::string& name) {
    if (name == "sdbm") return "SDBM Hash";
    if (name == "custom1") return "Custom Hash 1";
    if (name == "custom2") return "Custom Hash 2";
    return "SDBM Hash";
}
