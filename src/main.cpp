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
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>

#include "iocache.h"
#include "link_info.h"
#include "HciReader.h"
#include "Parser.h"
#include "ParserFactory.h"
//using namespace std;

#define SYSLOG_EVENT Oxffff

unsigned int guItemNum = 0;
u_int64 time_us = 0;

unsigned char linkObjNum = 0xff;

#define LINK_INFO_ENABLE

#ifdef LINK_INFO_ENABLE
unsigned char temp_handle[2] = {0,0};
unsigned char temp_bt_addr[6] = {0,0,0,0,0,0};
unsigned char temp_role = UNVALIBLE_ROLE;
#endif

link_info *linkobjPointer[8];

link_info Linkobj32(temp_handle,UNVALIBLE_PROTOCOL,UNVALIBLE_MODE,UNVALIBLE_ROLE,temp_bt_addr);
link_info Linkobj33(temp_handle,UNVALIBLE_PROTOCOL,UNVALIBLE_MODE,UNVALIBLE_ROLE,temp_bt_addr);
link_info Linkobj34(temp_handle,UNVALIBLE_PROTOCOL,UNVALIBLE_MODE,UNVALIBLE_ROLE,temp_bt_addr);
link_info Linkobj35(temp_handle,UNVALIBLE_PROTOCOL,UNVALIBLE_MODE,UNVALIBLE_ROLE,temp_bt_addr);
link_info Linkobj0200(temp_handle,UNVALIBLE_PROTOCOL,UNVALIBLE_MODE,UNVALIBLE_ROLE,temp_bt_addr);
link_info Linkobj0201(temp_handle,UNVALIBLE_PROTOCOL,UNVALIBLE_MODE,UNVALIBLE_ROLE,temp_bt_addr);
link_info Linkobj0202(temp_handle,UNVALIBLE_PROTOCOL,UNVALIBLE_MODE,UNVALIBLE_ROLE,temp_bt_addr);
link_info Linkobj0203(temp_handle,UNVALIBLE_PROTOCOL,UNVALIBLE_MODE,UNVALIBLE_ROLE,temp_bt_addr);

void intialise_link_obj_pontiers(void)
{
    linkobjPointer[0] = &Linkobj32;
    linkobjPointer[1] = &Linkobj33;
    linkobjPointer[2] = &Linkobj34;
    linkobjPointer[3] = &Linkobj35;
    linkobjPointer[4] = &Linkobj0200;
    linkobjPointer[5] = &Linkobj0201;
    linkobjPointer[6] = &Linkobj0202;
    linkobjPointer[7] = &Linkobj0203;
    
    temp_handle[0] = 0x0;
    temp_handle[1] = 0x32;
    
    linkobjPointer[0]->update_link_handle(temp_handle);
    
    temp_handle[1] = 0x33;
    linkobjPointer[1]->update_link_handle(temp_handle);
    
    temp_handle[1] = 0x34;
    linkobjPointer[2]->update_link_handle(temp_handle);
    
    temp_handle[1] = 0x35;
    linkobjPointer[3]->update_link_handle(temp_handle);
    
    temp_handle[0] = 0x02; 
    temp_handle[1] = 0x00;
    linkobjPointer[4]->update_link_handle(temp_handle);
    
    temp_handle[1] = 0x01;
    linkobjPointer[5]->update_link_handle(temp_handle);
    
    temp_handle[1] = 0x02;
    linkobjPointer[6]->update_link_handle(temp_handle);
    
    temp_handle[1] = 0x03;
    linkobjPointer[7]->update_link_handle(temp_handle);
}

unsigned char find_linkObj_by_handle(unsigned char handle[2])
{
    unsigned char i;
    for(i=0; i<8; i++){
        if(linkobjPointer[i]->check_handle(handle)){
            return i;
        }
    }
    return 0xff;
}

unsigned char find_linkObj_by_btaddr(unsigned char bt_addr[6])
{
    unsigned char i;
    for(i=0; i<8; i++){
        if(linkobjPointer[i]->check_btaddr(bt_addr)){
            return i;
        }
    }
    return 0xff;
}

int print_time_tag(u_int64 tm, FILE* fd, int timeoffset)
{
    struct timeval tv;
    char s[20];
    //printf("hcitime_to_str\n");
    
    if (tm < 0)
    {
        //strcpy(out, "inualid time");
        return 1;
    }
    tv.tv_sec = tm / 1000000 + 8*3600; //adjust +8 hour for Chinese time zone
    tv.tv_usec = tm % 1000000;
    time_t tt = tv.tv_sec;
    //need show as main log , so need cast to GMT
    tt += timeoffset * 60;
    
    if (tt < 0)
    {
    // strcpy(out, "inualid tine");
     return 1;
    }
    
    struct tm* tl = gmtime(&tt);
    if (!tl)
    {
        // strcpy(out, "inualid tine");
         return 1;
    }
    
    float sec = tl->tm_sec + (float)tv.tv_usec / 1000000;
    //return sprintf(out, "%02d-%02d %02d:%02d:%09.6f ", tl->tn_non + 1, tl->tn_mday, t] >tn_hour, tl->tn_uiin, sec);
    fprintf(fd,"%02d-%02d %02d:%02d:%02.0f|", tl->tm_mon + 1, tl->tm_mday, tl->tm_hour, tl->tm_min, sec);
}

void printf_tag(FILE* fd)
{
#if 0
    Link0bj32.print_link_info(fd);
    Link0bj33.print_link_info(fd);
    Link0bj34.print_link_info(fd);
    Link0bj35.print_link_info(fd);
    Link0bj2001.print_link_info(fd);
    Link0bj2002.print_link_info(fd);
    Link0bj2003.print_link_info(fd);
    Link0bj2004.print_link_info(fd);
#endif

    int i = 0;
    print_time_tag(time_us, fd, 0);
    for(i=0;i<8;i++)
    {
        linkobjPointer[i]->print_link_info(fd);
    }
    fprintf(fd, "%8d <->", guItemNum);
}

int64 CastSnooftime(unsigned char time_arry[8])
{
    u_int64 time = 0;
    unsigned char* p;
    //printf("CastSnooftine \n");
    p = &time_arry[0];
    u_int32 t1 = TOINT(p);
    p = &time_arry[4];
    u_int32 t2 = TOINT(p);
    time = (u_int64)t1 << 32 | t2;
    time -= 0x00dcddb30f2f8000ULL;
    return time;
}

int runit(io_cache* raw, io_cache* result_log, FILE* fd)
{
    if(!raw || !result_log) return 0;
    //first ignore the snoop log head of 16 bytes;
    int begin = 16;
    int time_begin = 16;
    int length = 0;
    unsigned char data[2048];
    unsigned char time[8];
    unsigned char* p;
    unsigned char pkg_type;
    int err = -1;
    printf("please wait... \n");
    //first write hci log head to hci log and syslog
    //raw->copyto( out_hci, 0 , 16 );
    //raw->copyto( result_log, 0 , 16 );
    fprintf(fd,"         |--|--|--|--|--|--|--|--|--|\n"); 
    do
    {
        guItemNum++;
        err = raw->read(begin , 25 , data );
        begin += 25;
        
        time_us = CastSnooftime(&data[16]); //get time for snoop log
        if(err <= 0 ) break;
        p = &data[4];
        length = TOINT( p ); //pkg len
        pkg_type = data[24]; //pkg type
        //err = n_read->read( begin , length - 1 ydata );
        err = raw->read( begin , length - 1 ,data );
        if( err <= 0 ) break;
        begin += (length-1);
        switch( pkg_type )
        { 
            case 0x04: // process euent information
                 //deal_evt_pkg(data, fd);
                 break;
            
            case 0x01: // process command information
                 //deal_cmd_pkg(data, fd);
                 break;
           default: break;
        }
    }while(1);
    
    result_log->flush();
    return err < 0 ? 0:1;
}

void filename( char* path , char* name , char* dir)
{
    if( !path ) return;
    char* p, *p1;
    p = path + strlen( path ) - 1;
    p1 = p;
    //get the last '.'
    while( p1 > path && *p1 != '.' ) p1--;
    if( p1 <= path ) p1 = p;
    
    while( p > path && *p != '\\' && *p != '/' ) p--;
    while( *path && path <= p ) *dir++ = *path++;
    *dir = '\0';
    p++;
    
    while( *p && p < p1 ) *name++ = *p++;
    *name = '\0';
}
int main(int argc, char **argv)
{
    char fname[255];
    char dir[512];
    char hcipath[512];
    char syslogpath[512];
    FILE* rawfd, *hcifd, *resultfd;
    
    /*
     * prepare file name
     */
    filename( argv[1] , fname , dir);

#if (BUILD_TYPE == 0) //For page timeout
#pragma message(0)
    sprintf ( syslogpath ,"%s%s-pagetineout.txt" ,dir ,fname );
#endif

#if (BUILD_TYPE == 1) //For inquriy
#pragma message(1)
    sprintf( syslogpath ,"%s%s-inquiry.txt" ,dir ,fname );
#endif

#if (BUILD_TYPE == 2) //For synchronizg
#pragma message(2)
    sprintf( syslogpath ,"%s%s-synchronize.txt" ,dir ,fname );
#endif

#if (BUILD_TYPE == 3) //For synchronize
#pragma message(2)
    sprintf( syslogpath ,"%s%s-spcial_WCNCR0O202989.txt" ,dir ,fname );
#endif

    /*
     * open resource and result files
     */
    if(!((rawfd = fopen( argv[1] ,"rb+")) &&
        //(hcifd = fopen( hcipath ��"wb+")) &&
        (resultfd= fopen(syslogpath ,"wb+"))
        ))
    {
        printf("open file error\n");
        return 1;
    }
    
    /*
     *buffers
     */
    io_cache raw(rawfd , 1024*1024*4 ); //
    io_cache result(resultfd, 1024*1024*4);
    
    /*
     * initial relative object
     */
    intialise_link_obj_pontiers();
    
    //hci line hci_log(&raw ,&result, resultfd);
    hci_line hci_log(&raw);
    
    /*
     *creat paser [Factory Design Pattern]
     */
    
//#ifdef BUILD_TVPE
#if (BUILD_TVPE == 0)
    ConnectionFactory parser;
#endif

#if (BUILD_TVPE == 1)
    SearchingFactory parser;
#endif

    //flEperfornanceFactory parser;
#if (BUILD_TVPE == 2)
    SynchronousLinkFactory parser;
#endif

#if (BUILD_TVPE == 3)
    SpecialFactory parser;
#endif
    
    Parser* new_parser = parser.CreateParser();
    /*
     * run parser
     */
    new_parser->RunParsing(&raw ,&result, resultfd, hci_log);
    return 0;
}
