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
#ifndef PARSER_HEAD
#define PARSER_HEAD

#include <stdio.h>
#include "typedef.h"
#include "iocache.h"
#include "HciReader.h"

/*
 *op code micro define
 */
#define HCI_Invild_opcode                       0xffff
#define HCI_Read_Local_Version_lnformation      0x1001
#define HCI_Read_BD_ADDR                        0x1009
#define HCI_LE_Extended_Create_Connection       0x2043
//#define HCI_Sniff_Hode0x0203
#define HCI_INQUIRY                             0x0401
#define HCI_Inquiry_Cancel                      0x0402
#define HCI_CREATE_CONNECTION                   0x0405
#define HCI_ACCEPT_CONNECTION_REQUEST           0x0409
#define HCI_REJECT_CONNECTION_REQUEST           0x040A
#define HCI_LINK_KEY_REQUEST_REPLY              0x040B
#define HCI_REMOTE_NAME_REQUEST                 0x0419
#define HCI_Remote_Name_Request_Cancel          0x041A
#define HCI_SETUP_SYNCHRONOUS_CONNECTION        0x0428
#define HCI_ENHANCED_SETUP_SYNCHRONOUS_CONNECTION 0x043d
#define HCI_LE_SET_SCAN_ENABLE                  0x200c
#define HCI_LE_SET_EXTENDED_SCAN_ENABLE         0x2042

/*-----------------------
event code micro define
-------------------------*/
#define EVENT_Invild_event                                     0xff
#define EVENT_INQUIRY_COMPLETE                                 0x01
#define EVENT_HCI_INQUIRY_RESULT                               0x02
#define EVENT_Connection_Complete                              0x03
#define EVENT_Disconnection_Complete                           0x05
#define EVENT_Remote_Name_Request_Complete                     0x07
#define EVENT_Read_Remote_Version_Information_Complete         0x0C
#define EVENT_Command_Complete                                 0x0E
#define EVENT_Command_Status                                   0x0F
#define EVENT_HCI_Role_Change                                  0x12
#define EUENT_Mode_Change                                      0x14
#define EVENT_Link_Key_Notification                            0x18
#define EVENT_HCI_Inquiry_Result_with_RSSI                     0x22
#define EVENT_HCI_Synchronous_Connection_Complete              0x2C
#define EVENT_HCI_Extended_Inquiry_Result                      0x2F
#define EVENT_LE_Meta_Event                                    0x3E

/*
 * GOF Factory Method Design pattern
 */

//Abstract Base Class
class Parser
{
public:
    virtual void RunParsing(io_cache* raw , io_cache* result_log, FILE* fd, hci_line hci_log);
};

class ConnectionTimeoutParser : public Parser
{
public:
    void RunParsing(io_cache* raw , io_cache* result_log, FILE* fd, hci_line hci_log);
};

class ConnectionParser : public Parser
{
public:
    void RunParsing(io_cache* raw , io_cache* result_log, FILE* fd, hci_line hci_log);
};

class SearchingParser : public Parser
{
public:
    void RunParsing(io_cache* raw , io_cache* result_log, FILE* fd, hci_line hci_log);
};
/*
 * GOF Chain of Reponsiblility design pattern *1
 */

typedef int t_opcode;
const t_opcode INVLID_OPCODE = -1;
const t_opcode opcode_CreatConnection = 0x0405;
const t_opcode opcode_AcceptConnection = 0x0409;
//Abstract Base Class
class ParserHandler
{
    public:
        ParserHandler(ParserHandler* = 0);
        virtual bool HasParser();
        void SetHandler(ParserHandler*, t_opcode);
        virtual void HandlePaser(t_opcode opcode, u_int8* data, FILE* fd);
        t_opcode GetOpcode(void);
        void SetOpcode(t_opcode op_code);
        ParserHandler* GetSuccessor(void);
    private:
        ParserHandler* _successor;
        t_opcode _opcode;
};

/*
 *HCI command component
 */
class hci_cmd_entry : public ParserHandler
{
    public:
        hci_cmd_entry(ParserHandler* p);
        void HandlePaser(t_opcode op_code, u_int8* data, FILE* fd);
};


class HCI_Inquiry : public ParserHandler
{
    public:
        HCI_Inquiry(ParserHandler* p);
        void HandlePaser(t_opcode op_code, u_int8* data, FILE* fd);
};

class HCI_Create_Connection : public ParserHandler
{
    public:
        HCI_Create_Connection(ParserHandler* p);
        void HandlePaser(t_opcode opcode, u_int8* data, FILE* fd);
};

class HCI_Accept_Connection_Request : public ParserHandler
{
    public:
        HCI_Accept_Connection_Request(ParserHandler* p);
        void HandlePaser(t_opcode opcode, u_int8* data, FILE* fd);
};

class HCI_Reject_Connection_Request : public ParserHandler
{
    public:
        HCI_Reject_Connection_Request(ParserHandler* p);
        void HandlePaser(t_opcode op_code, u_int8* data, FILE* fd);
};

class HCI_Link_Key_Request_Reply : public ParserHandler
{
    public:
        HCI_Link_Key_Request_Reply(ParserHandler* p);
        void HandlePaser(t_opcode op_code, u_int8* data, FILE* fd);
};

class HCI_Remote_Name_Request : public ParserHandler
{
    public:
        HCI_Remote_Name_Request(ParserHandler* p);
        void HandlePaser(t_opcode op_code, u_int8* data, FILE* fd);
};

class HCI_Setup_Synchronous_Connection : public ParserHandler
{
    public:
        HCI_Setup_Synchronous_Connection(ParserHandler* p);
        void HandlePaser(t_opcode op_code, u_int8* data, FILE* fd);
};

class HCI_Enhanced_Setup_Synchronous_Connection : public ParserHandler
{
    public:
        HCI_Enhanced_Setup_Synchronous_Connection(ParserHandler* p);
        void HandlePaser(t_opcode op_code, u_int8* data, FILE* fd);
};

class HCI_LE_Set_Scan_Enable : public ParserHandler
{
    public:
        HCI_LE_Set_Scan_Enable(ParserHandler* p);
        void HandlePaser(t_opcode op_code, u_int8* data, FILE* fd);
};

class HCI_LE_Set_Extended_Scan_Enable : public ParserHandler
{
    public:
        HCI_LE_Set_Extended_Scan_Enable(ParserHandler* p);
        void HandlePaser(t_opcode op_code, u_int8* data, FILE* fd);
};

/**************************/

class hci_event_entry : public ParserHandler
{
    public:
        hci_event_entry(ParserHandler* p);
        void HandlePaser(t_opcode op_code, u_int8* data, FILE* fd);
};

class EVENT_Inquiry_Complete: public ParserHandler
{
    public:
        EVENT_Inquiry_Complete(ParserHandler* p);
        void HandlePaser(t_opcode op_code, u_int8* data, FILE* fd);
};
class EVENT_HCI_Inquiry_Result : public ParserHandler
{
    public:
        EVENT_HCI_Inquiry_Result(ParserHandler* p);
        void HandlePaser(t_opcode op_code, u_int8* data, FILE* fd);
};

#endif
