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
#include "link_info.h"
#include<stdlib.h>
#include<memory.h>

link_info::link_info(unsigned char handle[2], unsigned char  protocl, unsigned char mode, unsigned char role,unsigned char bt_addr[6])
{
    this->handle[0] = handle[0];
    this->handle[1] = handle[1];

    this->protocol = protocol;
    this->role = role;
    this->mode = mode; 

    this->bt_addr[0] = bt_addr[0];
    this->bt_addr[1] = bt_addr[1];
    this->bt_addr[2] = bt_addr[2];
    this->bt_addr[3] = bt_addr[3];
    this->bt_addr[4] = bt_addr[4];
    this->bt_addr[5] = bt_addr[5];
}

 
link_info::~link_info()
{
    this->handle[0] = 0;
    this->handle[1] = 0;
    
    this->protocol = UNVALIBLE_PROTOCOL;
    this->role = UNVALIBLE_ROLE;
    this->mode = UNVALIBLE_MODE;

    this->bt_addr[0] =0; 
    this->bt_addr[1] =0; 
    this->bt_addr[2] =0; 
    this->bt_addr[3] =0; 
    this->bt_addr[4] =0; 
    this->bt_addr[5] =0; 
} 

void link_info::update_link_role(unsigned char role)
{
    this->role = role;
}

void link_info::update_link_mode(unsigned char mode)
{
    this->mode = mode;
}

void link_info::update_link_protocol(unsigned char protocol)
{
    this->protocol = protocol;
}

bool link_info::check_handle(unsigned char handle[2])
{
    if((this->handle[0] == handle[0]) && (this->handle[1] == handle[1]))
    {
        return true;
    }
    return false;
}

bool link_info::check_btaddr(unsigned char bt_addr[6])
{
    if((this->bt_addr[0] == bt_addr[0]) &&\
       (this->bt_addr[1] == bt_addr[1]) &&\
       (this->bt_addr[2] == bt_addr[2]) &&\
       (this->bt_addr[3] == bt_addr[3]) &&\
       (this->bt_addr[4] == bt_addr[4]) &&\
       (this->bt_addr[5] == bt_addr[5])){ 
            return true;
    }
        return false;
}

void link_info::print_link_info(FILE* fd)
{
    if(BLE == this->protocol){
        if(MASTER == this->role){
            fprintf(fd,"* |");
    }
        else if(SLAVE == this->role){
            fprintf(fd," *|");
        }
    }
    else if(BT == this->protocol){
        if(ACTIUE == this->mode){
            if(MASTER == this->role){
                fprintf(fd,"+ |");
        }else if(SLAVE == this->role){
                fprintf(fd," +|");
        }
        }else if(SNIFF == this->mode){
            if(MASTER == this->role){
                fprintf(fd, "* |");
            }else if(SLAVE == this->role){
                fprintf(fd," *|");
            }
        }
    }
    else{
    fprintf(fd,"  |");
    }
}

void link_info::update_link_handle(unsigned char handle[2])
{
    this->handle[0] = handle[0];
    this->handle[1] = handle[1];
}

void link_info::update_link_btaddr(unsigned char bt_addr[2])
{
    this->bt_addr[0] = bt_addr[0];
    this->bt_addr[1] = bt_addr[1];
    this->bt_addr[2] = bt_addr[2];
    this->bt_addr[3] = bt_addr[3];
    this->bt_addr[4] = bt_addr[4];
    this->bt_addr[5] = bt_addr[5];
}
