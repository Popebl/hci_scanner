/*
 *
 * Copyright (C) 2023 Bo Lei
 *
 * This file is part of HCI scanner.
 *
 * HCI scanner is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * HCI scanner is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */
#ifndef IOCACHE_HEAD
#define IOCACHE_HEAD
#include<stdio.h>
class io_cache
{  
    public:
    io_cache(FILE* fd, int size);
    ~io_cache();
    int write( unsigned char* data , int len);
    int flush();
    int read(int start, int count, unsigned char* buff);
    int copyto(io_cache* dest, int start, int count);
    int filelength(){return filesize;}
    
    private:
    FILE* fd;
    unsigned char* buffer;
    int size;
    int length;
    int fpos;
    int filesize;
};

#endif 
