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

#ifndef HCI_READER_HEAD
#define HCI_READER_HEAD

#include "iocache.h"
#include "typedef.h"
#include<stdio.h>

typedef struct {
    //u_int8 message_type; 
    u_int16 opcode;
    u_int8 Parameter_Total_Length;
    u_int8* Parameter;
} t_hci_command;


typedef struct{ 
    //u_int8 message_type;
    u_int8 event_code;
    u_int8 Parameter_Total_Length;
    u_int8* Parameter;
} t_hci_event;

class hci_line
{
    public:
    int begin = 16;
    int time_begin = 16;
    unsigned char data[2048];
    unsigned char time[8];
    unsigned char pkgType;
    unsigned int guItemNum;
    unsigned long time_us; 
    io_cache* raw;
    //io_cache* result_log;
    //FILE* fd;
    
    //hci_line(io_cache* raw , iocache* resultlog, FILE* fd);
    hci_line(io_cache* raw);
    bool get_next_line(void);
    u_int32 get_line_num(void);
    u_int64 get_time_stamp(void);
    u_int64 get_pkg_type(void);
    u_int8* get_data(void);
};

#endif
