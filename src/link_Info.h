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
 */s
#ifndef LINKINFO_HEAD
#define LINKINFO_HEAD
#include<stdio.h>

/*------- Link Protocal -----------*/
#define UNVALIBLE_PROTOCOL (0)
#define BT                 (1)
#define BLE                (2)
 
/*----- Link Role -------*/
#define UNVALIBLE_ROLE     (0)
#define MASTER             (1)
#define SLAVE              (2)

/*-----Link mode---------*/
#define UNVALIBLE_MODE      (0)
#define ACTIUE             (1)
#define SNIFF              (2)

#define LINK_INFO_ENABLE

class link_info
{
    public:
        link_info(unsigned char handle[2], unsigned char protocol, unsigned char mode, unsigned char role, unsigned char bt_addr[6]);
       ~link_info();

       void print_link_info(FILE* fd);
       void update_link_handle(unsigned char handle[2]);
       void update_link_btaddr(unsigned char bt_addr[6]);
       void update_link_role(unsigned char role); 
       void update_link_mode(unsigned char mode);
       void update_link_protocol(unsigned char protocol);
       bool check_handle(unsigned char handle[2]);
       bool check_btaddr(unsigned char bt_addr[6]);
    private:
        unsigned char handle[2];
        unsigned char protocol; // '*' present low power
        unsigned char role;     // left present master, right present slave
        unsigned char mode;     // '+' present active, '*' present low power
        unsigned char bt_addr[6];
};

#endif
