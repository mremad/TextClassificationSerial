//
//  superHash.h
//  text2
//
//  Created by Moumen Mohamed on 6/15/14.
//  Copyright (c) 2014 Moumen Mohamed. All rights reserved.
//

#ifndef __text2__superHash__
#define __text2__superHash__

#include <iostream>

#endif /* defined(__text2__superHash__) */
#include "stdlib.h"
#include <stdio.h>
#include <stdint.h>
#include <string>


class SuperHash
{
    
    public :
    SuperHash();
     static  uint32_t create_hash( std::string data, int len,int tableSize);
     static  uint32_t hash_inc(const char * data, int len, uint32_t hash);
    
};