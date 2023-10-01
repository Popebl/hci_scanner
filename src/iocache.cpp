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
#include "iocache.h"
#include<stdlib.h>
#include<memory.h>
io_cache::io_cache( FILE* fd , int size )
{
    this->fd = fd;
    this->size = ( size + 4*1024 - 1 ) & (~(4*1024-1));
    buffer = (unsigned char*)malloc( size );
    fpos = 0;
    length = 0;
    //get file length
    fseek( this->fd, 0 ,SEEK_END );
    filesize = ftell( this->fd );
    fseek( this->fd, 0 ,SEEK_SET );
}

io_cache::~io_cache()
{
    // flush();
    fclose(fd);
    free(buffer);
}

int io_cache::write( unsigned char* data , int len )
{
    if( !data || len <= 0 ) return 0;
    if( size <= len )
    {
        //write to file directly;
        return fwrite( data , len , 1 , fd );
    }
    if( size - length < len )
    {
        //cache is not room, write to file
        flush();
    }
    memcpy( buffer + length , data , len );
    length += len;
    return len;
}

int io_cache::flush()
{
    fwrite( buffer ,length , 1, fd);
    length = 0; 
    return 1;
}

int io_cache::read( int start , int count ,unsigned char* buff )
{
    int len = 0;
    if( start < 0 || count <= 0 || start > filesize ) return 0;
    if( start < fpos || start > fpos + length )
    {
        //loading data from file;
        fseek( fd ,start , SEEK_SET );
        len = fread( buffer, sizeof(unsigned char), size ,fd );
        if( len <= 0 )
        {
            //the is not any data in the file
            return -1;
        }
        length = len;
        fpos = start;
    }
    len = length - start + fpos ; //can be readed in cache buffer;
    if( len >= count )
    {
        memcpy( buff ,buffer + start - fpos , count );
        return count;
    }else
    {
        memcpy( buff , buffer + start - fpos , len );
        //read the rest of data;
        int l = fread( buffer, sizeof(char), size ,fd );
        if( l <= 0 )
        {
            //is the end of file;
            return len;
        }
        fpos = fpos + length;
        length = l;
        memcpy( buff + len , buffer ,count - len );
    }
        return count;
}
        
int io_cache::copyto( io_cache* dest , int start , int count )
{
    unsigned char buff[1024];
    int n = count; 
    int len = 0;
    while( n - 1024 >= 0 )
    {
        len = this->read( start, 1024, buff );
        if( len > 0 )
            dest->write( buff,len );
        n -= 1024;
        start += 1024;
    }
    len = count & 1023;
    len = this->read( start, len, buff );
    if( len > 0 )
        dest->write( buff , len );
    return 1;
}
