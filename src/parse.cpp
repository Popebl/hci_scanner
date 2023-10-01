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
#include<stdlib.h>
#include<memory.h>
#include "Parser.h"
#include "link_info.h"

extern int runit(io_cache* raw, io_cache* result_log, FILE* fd);
//extern hci_line hci_log;
#ifdef LINK_INFO_ENABLE
extern unsigned char temp_handle[2];
extern unsigned char temp_bt_addr[6];
extern unsigned char temp_role;
extern unsigned char linkObjNum;
extern link_info *linkobjPointer[8];
extern unsigned char find_linkObj_by_handle(unsigned char handle[2]);
extern unsigned char find_linkObj_by_btaddr(unsigned char bt_addr[6]);
#endif

extern unsigned int guItemNum;
extern u_int64 time_us;
extern void printf_tag(FILE* fd);

void Parser::RunParsing(io_cache* raw , io_cache* result_log, FILE* fd, hci_line hci_log)
{
#if (DEBUG_LEVEL == DEBUG)
    printf("Parser\n");
#endif
}

#define NEW_METHOD

void ConnectionTimeoutParser::RunParsing(io_cache* raw , io_cache* result_log, FILE* fd, hci_line hci_log)
{
#if (DEBUG_LEVEL == DEBUG)
    printf("ConnectionTimeoutParser\n");
#endif
}

unsigned long page_time_start = 0;
void ConnectionParser:: RunParsing(io_cache* raw , io_cache* result_log, FILE* fd, hci_line hci_log)
{
#if (DEBUG_LEVEL == DEBUG)
    printf("ConnectionParser\n");
#endif
    /*
     * Hci command responsibility chain
     */
    //ParserHandler* h_cmd_0405 = new hci_cmd_0x0405(0, HCI_Create_Connection);
    ParserHandler* h_cmd_0409 = new HCI_Accept_Connection_Request(0);
    //ParserHandler* h_cmd_0419 = new hci_cmd_0x0419(h_cmd_0409,HCI_Remote_Name_Request);
    ParserHandler* h_cmd_040a = new HCI_Reject_Connection_Request(h_cmd_0409);
    //ParserHandler* h_cmd_040b = new hci_cmd_Ox040b(h_cmd_040a,HCI_Link_Key_Request_Reply);
    ParserHandler* h_cmd_0401 = new HCI_Inquiry(h_cmd_040a);
    //ParserHandler* h_cmd_200c = new hci_cmd_0x200c(h_cmd_0401,HCI_LE_Set_Scan_Enable);
    ParserHandler* hci_cmd_entrence = new hci_cmd_entry(h_cmd_0401);

    /*
     * Hci event responsibility chain
     */
    ParserHandler* hci_event_01 = new EVENT_Inquiry_Complete(0);
    //ParserHandler* hci_event_03 = new hci_event_0x03(hci_event_01,EVENTConnectionConplete);
    //ParserHandler* hci_event_05 = new hci_event_0x05(hci_event_03,EVENTDisconnectionConplete);
    //ParserHandler* hci_event_07 = new hci_event_0x07(hci_event_05,EVENT Renote Nane Request Conplete);
    //ParserHandler* hci_event_0e = new hci_event_0x0e(hci_event_07,EVENTConrnandConplete);
    //ParserHandler* hci_event_0c = new hci_event_0x0c(hci_event_0e,Event_Read_Remote_Uersion_Information_Complete);
    //ParserHandler* hci_event_18 = new hci_event_0x18(hci_event_0c,EuentLinkKeyNotification);
    //ParserHandler* hci_event_0f = new hci_event_0x0f(hci_event_18,EuentConrnandStatus);
    //ParserHandler* hci_event_3e = new hci_event_0x3e(hci_event_0f,EVENTLEMetaEuent);
    ParserHandler* hci_event_entrentry = new hci_event_entry(hci_event_01);

    fprintf(fd, "           |--|--|--|--|--|--|--|--|--|\n");

    u_int32 loop_cnt = 0;
    while(hci_log.get_next_line())
    {
        loop_cnt++;
#if (DEBUG_LEVEL == DEBUG)
        printf("line %d to be processing     ", hci_log.get_line_num());
#endif
        guItemNum = hci_log.get_line_num() - 1;
        time_us = hci_log.get_time_stamp();
        int pkgtype = hci_log.get_pkg_type();
        unsigned char* data = hci_log.get_data();
        unsigned char* temp_char_pointer = NULL;

        switch(pkgtype)
        {
            /* process command information */
            case 0x01:
                t_hci_command hci_command;
                temp_char_pointer = &data[0];
                hci_command.opcode = LITTLE_ENDIAN_CHAR_TO_SH0RT(temp_char_pointer);
                hci_command.Parameter_Total_Length = data[2];
                hci_cmd_entrence->HandlePaser(hci_command.opcode,data,fd);
                break;

            /* process euent information */
            case 0x04:
                t_hci_event hci_event;
                hci_event.event_code = data[0];
                hci_event.Parameter_Total_Length = data[1];
                hci_event.Parameter = &data[2];
                hci_event_entrentry->HandlePaser(hci_event.event_code,data,fd);
                break;

            default:
#if (DEBUG_LEVEL == DEBUG)
                printf("\n");
#else
                ;
#endif
        }
    }
    fprintf(fd,"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nHci log scaner tool for searching feature");
    fprintf(fd,"\nGNU General Public License. ��Ҫ��ҵ��Ȩ");
    fprintf(fd,"\nPresented by pope.b.lei@gmail.com");
    fprintf(fd,"\nReport issue to https://github.com/Popebl/hci_scanner");
    fprintf(fd,"\nHave a pleasure");


}

void SearchingParser:: RunParsing(io_cache* raw , io_cache* result_log, FILE* fd, hci_line hci_log)
{
#if (DEBUG_LEVEL == DEBUG)
#endif
}

/*
class ParserHandler
{
    public:
        ParserHandler(ParserHandler* = 0, t_opcode = INVLID_OPCODE);
        virtual bool HasParser();
        void SetHandler(ParserHandler*, t_opcode);
        virtual void HandlePaser();

    private:
        ParserHandler* successor;
        t_opcode _opcode;
};
*/

unsigned long inquiry_time_start = 0;


