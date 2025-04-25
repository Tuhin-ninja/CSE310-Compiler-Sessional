#include "hash_functions.hpp"




unsigned int SDBMHash(const std::string& str, unsigned int num_buckets) { unsigned int hash = 0;
    unsigned int len = str.length();
    for (unsigned int i = 0; i < len; i++)
    {
    hash = ((str[i]) + (hash << 6) + (hash << 16) - hash) %
    num_buckets; }
    return hash;
    }

    unsigned int hashFNV1a(const std::string& str, unsigned int num_buckets) {
        const unsigned int seed = 0x9747b28c;
        const unsigned int m = 0x5bd1e995;
        const int r = 24;
    
        unsigned int len = str.length();
        const unsigned char* data = (const unsigned char*)str.c_str();
        unsigned int h = seed ^ len;
    
        while (len >= 4) {
            unsigned int k = *(unsigned int*)data;
    
            k *= m;
            k ^= k >> r;
            k *= m;
    
            h *= m;
            h ^= k;
    
            data += 4;
            len -= 4;
        }
    
        switch (len) {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
                h *= m;
        }
    
        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;
    
        return h % num_buckets;
    }
    

    unsigned int hashMurmurInspired(const std::string& str, unsigned int num_buckets) {
        unsigned int hash = 0;
    
        for (char ch : str) {
            hash ^= static_cast<unsigned int>(ch);
            hash *= 0x5bd1e995;
            hash ^= hash >> 15;
        }
    
        return hash % num_buckets;
    }
    

HashFunction getHashFunctionByName(std::string name) {
    if (name == "sdbm") return SDBMHash;
    if (name == "fnv1a") return hashFNV1a;
    if (name == "murmur") return hashMurmurInspired;
    return SDBMHash; // default
}


std::string getHashFunctionName(const std::string& name) {
    if (name == "sdbm") return "SDBM Hash";
    if (name == "custom1") return "Custom Hash 1";
    if (name == "custom2") return "Custom Hash 2";
    return "SDBM Hash";
}





