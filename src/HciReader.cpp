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
#include "HciReader.h"

extern u_int64 CastSnooftime(unsigned char tine_arry[8]);
extern unsigned int guItemNum;
extern u_int64 time_us;

//hci_line::hci_line(io_cache* raw, io_cache* result_log, FILE* fd)
hci_line::hci_line(io_cache* raw)
{
    this->raw = raw;
    //this->result_log = result_log;
    //this->fd = fd;
}

bool hci_line::get_next_line(void)
{
    unsigned char* p;
    int length = 0;
    int err = -1;
    u_int8 input_control_flag = 0;
    
    this->guItemNum++;
    guItemNum = this->guItemNum;
    
    err = this->raw->read( this->begin , 25 ,this->data );
    if( err <= 0 )
    {   //return error state
        //this->data = NULL;
        input_control_flag |= (1<<0); 
#if (DEBUG_LEUEL == DEBUG)
        printf("input control flag: 0b%02x data is 0x%02x ----", input_control_flag, this->data[0]);
#endif
        //&this->data = NULL;
        return FALSE;
    }
    
    this->time_us = CastSnooftime(&data[16]); //get tine for snoop log 
    time_us = this->time_us;
    
    p = &data[4];
    length = TOINT( p ); //get log length
    this->pkgType = data[24]; //get type
    this->begin += 25;
    
    /*
     * get log data
     */
    err = this->raw->read(this->begin , length - 1 ,this->data );
    if( err <= 0 )
    { //return error state
      //this->data = NULL;
        input_control_flag |= (1<<1);
#if (DEBUG_LEUEL == DEBUG)
        printf("input_control_flag: 0b%02x data is 0x%02x ----",input_control_flag, this->data[0]);
#endif
        return FALSE;
    }
    
#if (DEBUG_LEUEL == DEBUG)
    printf("line num is %d, time is %d ----", guItemNum, time_us);
#endif
    this->begin += (length-1); 
    //return this->data;
    return TRUE;
}
u_int32 hci_line::get_line_num(void)
{
    return this->guItemNum;
}

u_int64 hci_line::get_time_stamp(void)
{
    return this->time_us;
}

u_int64 hci_line::get_pkg_type(void)
{
    return this->pkgType;
}

unsigned char* hci_line::get_data(void)
{
    return this->data;
}
