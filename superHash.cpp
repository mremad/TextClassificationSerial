//
//  superHash.cpp
//  text2
//
//  Created by Moumen Mohamed on 6/15/14.
//  Copyright (c) 2014 Moumen Mohamed. All rights reserved.
//

#include "SuperHash.h"

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
|| defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
+(uint32_t)(((const uint8_t *)(d))[0]) )
#endif


SuperHash:: SuperHash()
{
    
}


 uint32_t SuperHash::hash_inc(const char * data, int len, uint32_t hash)
{
    uint32_t tmp;
    int rem;
    
    if (len <= 0 || data == NULL) return 0;
    
    rem = len & 3;
    len >>= 2;
    
    /* Main loop */
    for (;len > 0; len--) {
        hash  += get16bits (data);
        tmp    = (get16bits (data+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        data  += 2*sizeof (uint16_t);
        hash  += hash >> 11;
    }
    
    /* Handle end cases */
    switch (rem) {
        case 3: hash += get16bits (data);
            hash ^= hash << 16;
            hash ^= data[sizeof (uint16_t)] << 18;
            hash += hash >> 11;
            break;
        case 2: hash += get16bits (data);
            hash ^= hash << 11;
            hash += hash >> 17;
            break;
        case 1: hash += *data;
            hash ^= hash << 10;
            hash += hash >> 1;
    }
    
    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    
    return hash;
}

uint32_t SuperHash::create_hash(std::string data, int len, int tablesize)
{
    char * charList= (char *)malloc(len*sizeof(char));
    for(int i=0;i<len;i++)
        charList[i]= data[i];
    
    uint32_t result= hash_inc(charList, len, (uint32_t) len);
    free(charList);
    return result%tablesize;
}