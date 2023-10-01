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
#include "Parser.h"
#include "link_info.h"

#ifdef LINK_INFO_ENABLE
extern unsigned char temp_handle[2];
extern unsigned char temp_bt_addr[6];
extern unsigned char temp_role;
extern unsigned char linkObjNum;
extern link_info *linkobjPointer[8];
extern unsigned char find_linkObj_by_handle(unsigned char handle [2]);
extern unsigned char find_linkObj_by_btaddr(unsigned char bt_addr [6]);
#endif

extern unsigned int guItemNum;
extern u_int64 time_us;
extern void printf_tag(FILE * fd);
extern unsigned long inquiry_time_start;
extern unsigned long page_time_start;



ParserHandler::ParserHandler(ParserHandler* p): _successor(p) {}

//ParserHandler* hci_cmd_entrence= new hci_cmd_entry(h_cmd_0x40a, HCIInuildopcode);

bool ParserHandler::HasParser()
{
    return _opcode != INVLID_OPCODE;
}

void ParserHandler::SetOpcode(t_opcode op_code)
{
     _opcode = op_code;
}

t_opcode ParserHandler::GetOpcode(void)
{
    return _opcode;
}

ParserHandler* ParserHandler::GetSuccessor(void)
{
    return _successor;
}

void ParserHandler::HandlePaser(t_opcode op_code, u_int8* data, FILE* fd)
{
    if (_successor != 0)
    {
        _successor->HandlePaser(op_code, data, fd);
    }
}


/*
 * HCI command component *
 */
hci_cmd_entry::hci_cmd_entry(ParserHandler* p):ParserHandler(p)
{
    SetOpcode(INVLID_OPCODE);
}
void hci_cmd_entry::HandlePaser(t_opcode op_code, u_int8* data, FILE* fd)
{
#if (DEBUG_LEVEL == DEBUG)
    printf("hci_cmd_entry HandlePaser\n");
#endif
    ParserHandler::HandlePaser(op_code, data, fd);
}


HCI_Inquiry::HCI_Inquiry(ParserHandler* p):ParserHandler(p)
{
    SetOpcode(HCI_INQUIRY);
}
void HCI_Inquiry:: HandlePaser(t_opcode op_code, u_int8* data, FILE* fd)
{
#if (DEBUG_LEVEL == DEBUG)
    printf("HCI_Inquiry HandlePaser\n");
#endif
    if (GetOpcode() == op_code)
    {
       temp_role = MASTER;
       inquiry_time_start = time_us;
       printf_tag(fd);
       fprintf(fd,"Inquiry Command set %0.1fs length\n", 1.28*data[6]);
    }
    else if (GetSuccessor() != 0)
    {
        ParserHandler::HandlePaser(op_code, data, fd);
    }
}


HCI_Create_Connection::HCI_Create_Connection(ParserHandler* p):ParserHandler(p)
{
    SetOpcode(HCI_CREATE_CONNECTION);
}
void HCI_Create_Connection::HandlePaser(t_opcode op_code, u_int8* data, FILE* fd)
{
#if (DEBUG_LEUEL == DEBUG)
    printf ("HCI_Create_Connection HandlePaser\n");
#endif
    if (GetOpcode() == op_code)
    {
        temp_role = MASTER;
        printf_tag(fd);
        fprintf(fd,"Creat BT connection to 0x%02x:%02x:%02x:%02x:%02x:%02x\n,",data[8],data[7],data[6],data[5],data[4],data[3]);
        page_time_start = time_us;
    }
    else if (GetSuccessor() != 0)
    {
        ParserHandler::HandlePaser(op_code, data, fd);
    }
}


HCI_Accept_Connection_Request::HCI_Accept_Connection_Request(ParserHandler* p):ParserHandler(p)
{
    SetOpcode(HCI_ACCEPT_CONNECTION_REQUEST);
}
void HCI_Accept_Connection_Request::HandlePaser(t_opcode op_code, u_int8* data, FILE* fd)
{
//ParserHandler* successor = GetSuccessor(); t_opcode opcode = GetOpcode();
#if (DEBUG_LEUEL == DEBUG)
    //printf("HCI_Accept_Connection_Request HandlePaser opcode is 0x%04x input opcode is 0x%04x . successor opcode is 0x%04x    ", _opcode, op_code, _successor->GetOpcode());
#endif
    if (GetOpcode() == op_code)
    {
        //printf("1\n__);
        temp_role = SLAVE;
        printf_tag(fd);
        fprintf(fd,"Accept BT connection form 0x%02x:%02x:%02x:%02x:%02x:%02x    ", data[8],data[7],data[6],data[5],data[4],data[3]);
        if (0 == data[9]){
            fprintf(fd,"as MASTER\n");
        }else if(1 == data[9]){
            fprintf(fd,"as SLAVE\n");}
    }
    else if (GetSuccessor() != 0)
    {
        //printf("2\n");
        //_successor->HandlePaser(op_code, data, fd);
        ParserHandler::HandlePaser(op_code, data, fd);
    }
    else
    {
        //printf("3\n__);
    }
}


HCI_Reject_Connection_Request::HCI_Reject_Connection_Request(ParserHandler* p):ParserHandler(p)
{
    SetOpcode(HCI_REJECT_CONNECTION_REQUEST);
}
void HCI_Reject_Connection_Request::HandlePaser(t_opcode op_code, u_int8* data, FILE* fd)
{
#if (DEBUG_LEVEL == DEBUG)
    printf("HCI_Reject_Connection_Request HandlePaser\n");
#endif
    if (GetOpcode() == op_code)
    {
        printf_tag(fd);
        fprintf(fd,"[Fail] Connection Rejected by Host");
    }
    else if (GetSuccessor() != 0)
    {
        ParserHandler::HandlePaser(op_code, data, fd);
    }
}


HCI_Link_Key_Request_Reply::HCI_Link_Key_Request_Reply(ParserHandler* p) :ParserHandler(p)
{
    SetOpcode(HCI_LINK_KEY_REQUEST_REPLY);
}
void HCI_Link_Key_Request_Reply::HandlePaser(t_opcode op_code, u_int8* data, FILE* fd)
{
#if (DEBUG_LEVEL == DEBUG)
    printf("HCI_Link_Key_Request_Reply HandlePaser\n");
#endif
    if (GetOpcode() == op_code)
    {
        ParserHandler::HandlePaser(op_code, data, fd);
    }
}


HCI_Remote_Name_Request::HCI_Remote_Name_Request(ParserHandler* p) :ParserHandler(p)
{
    SetOpcode(HCI_REMOTE_NAME_REQUEST);
}
void HCI_Remote_Name_Request::HandlePaser(t_opcode op_code, u_int8* data, FILE* fd)
{
#if (DEBUG_LEVEL == DEBUG)
    printf("HCI_Remote_Name_Request HandlePaser\n");
#endif
    if (GetOpcode() == op_code)
    {
        printf_tag(fd);
        fprintf(fd,"RNR to 0x%02x:%02x:%02x:%02x:%02x:%02x command",data[8],data[7],data[6],data[5],data[4],data[3]);
        switch(data[9])
        {
            case 1:fprintf(fd,"PageScan Mode: R1");break;
            default:;
        }
        fprintf(fd,"\n");
    }
    else if (GetSuccessor() != 0)
    {
        ParserHandler::HandlePaser(op_code, data, fd);
    }
}


HCI_Setup_Synchronous_Connection::HCI_Setup_Synchronous_Connection(ParserHandler* p):ParserHandler(p)
{
    SetOpcode(HCI_SETUP_SYNCHRONOUS_CONNECTION);
}
void HCI_Setup_Synchronous_Connection::HandlePaser(t_opcode op_code, u_int8* data, FILE* fd)
{
#if (DEBUG_LEVEL == DEBUG)
    printf("HCI_Setup_Synchronous_Connection HandlePaser\n");
#endif
    if (GetOpcode() == op_code)
    {
        ParserHandler::HandlePaser(op_code, data, fd);
    }
}


HCI_Enhanced_Setup_Synchronous_Connection::HCI_Enhanced_Setup_Synchronous_Connection(ParserHandler* p):ParserHandler(p)
{
    SetOpcode(HCI_ENHANCED_SETUP_SYNCHRONOUS_CONNECTION);
}
void HCI_Enhanced_Setup_Synchronous_Connection::HandlePaser(t_opcode op_code, u_int8* data, FILE* fd)
{
#if (DEBUG_LEVEL == DEBUG)
    printf("HCI_Enhanced_Setup_Synchronous_Connection HandlePaser\n");
#endif
    if (GetOpcode() == op_code)
    {
        printf_tag(fd);
        fprintf(fd,"Enhance setup synchronous link on 0x%02x%02x\n",data[4],data[3]);
    }
    else if (GetSuccessor() != 0)
    {
        ParserHandler::HandlePaser(op_code, data, fd);
    }
}


HCI_LE_Set_Scan_Enable::HCI_LE_Set_Scan_Enable(ParserHandler* p):ParserHandler(p)
{
    SetOpcode(HCI_LE_SET_SCAN_ENABLE);
}
void HCI_LE_Set_Scan_Enable::HandlePaser(t_opcode op_code, u_int8* data, FILE* fd)
{
#if (DEBUG_LEVEL == DEBUG)
    printf ("HCI_LE_Set_Scan_Enable HandlePaser\n");
#endif
    if (GetOpcode() == op_code)
    {
        printf_tag(fd);
        switch(data[3])
        {
            case 0: fprintf(fd,"Host Disable BLE scan\n");break;
            case 1: fprintf(fd,"Host Enable BLE scan\n");break;
            default:;
        }
    }
    else if (GetSuccessor() != 0)
    {
        ParserHandler::HandlePaser(op_code, data, fd);
    }
}


HCI_LE_Set_Extended_Scan_Enable::HCI_LE_Set_Extended_Scan_Enable(ParserHandler* p):ParserHandler(p)
{
    SetOpcode(HCI_LE_SET_EXTENDED_SCAN_ENABLE);
}
void HCI_LE_Set_Extended_Scan_Enable:: HandlePaser(t_opcode op_code, u_int8* data, FILE* fd)
{
#if (DEBUG_LEVEL == DEBUG)
    printf("HCI_LE_Set_Extended_Scan_Enable HandlePaser\n");
#endif
    if (GetOpcode() == op_code)
    {
        printf_tag(fd);
        switch(data[3])
        {
            case 0:fprintf(fd,"Host Disable BLE scan, ");break;
            case 1:fprintf(fd,"Host Enable BLE scan, ");break;
            default:;
        }
        switch(data[4])
        {
            case 0:fprintf(fd,"Duplicate filtering disabled, ");break;
            case 1:fprintf(fd,"Duplicate filtering enabled, ");break;
            case 2:fprintf(fd,"Duplicate filtering enabled, reset for each scan period");
            default:;
        }
        fprintf(fd,"Duration 0x%02x%02xf Period 0x%02x%02x \n", data[6] ,data[5] ,data[8] ,data[7]);
    }
}


/*
* HCI event component *
*/
hci_event_entry::hci_event_entry(ParserHandler* p):ParserHandler(p)
{
    SetOpcode(INVLID_OPCODE);
}
void hci_event_entry:: HandlePaser(t_opcode op_code, u_int8* data, FILE* fd)
{
#if (DEBUG_LEVEL == DEBUG)
    printf("hci_event_entry HandlePaser\n");
#endif
    //if (GetSuccessor()!= 0)
    //{
        ParserHandler::HandlePaser(op_code, data, fd);
    //}
}


EVENT_Inquiry_Complete::EVENT_Inquiry_Complete(ParserHandler* p):ParserHandler(p)
{
    SetOpcode(EVENT_INQUIRY_COMPLETE);
}
void EVENT_Inquiry_Complete::HandlePaser(t_opcode op_code, u_int8* data, FILE* fd)
{
#if (DEBUG_LEVEL == DEBUG)
    printf("EVENT_Inquiry_Complete HandlePaser\n");
#endif
    if(GetOpcode() == op_code)
    {
        switch(data[2])
        {
            case 0:
                printf_tag(fd);
                fprintf(fd,"Inquiry End");
                fprintf(fd," inquiry tine is %d.%d seconds\n", (time_us- inquiry_time_start)/1000000, ((time_us- inquiry_time_start)/1000000)*100);
        }
    }
    else if (GetSuccessor() != 0)
    {
        ParserHandler::HandlePaser(op_code, data, fd);
    }
}


EVENT_HCI_Inquiry_Result::EVENT_HCI_Inquiry_Result(ParserHandler* p):ParserHandler(p)
{
    SetOpcode(EVENT_HCI_INQUIRY_RESULT);
}
void EVENT_HCI_Inquiry_Result:: HandlePaser(t_opcode op_code, u_int8* data, FILE* fd)
{
#if (DEBUG_LEVEL == DEBUG)
    printf("EVENT_HCI_Inquiry_Result HandlePaser\n");
#endif
if (GetOpcode() == op_code)
    {
        printf_tag(fd);
        fprintf(fd,"EIR:BT Addr is 0x%02x:%02x:%02x0x:%02x:%02x",data[7],data[6],data[5],data[4],data[3],data[2]);
        switch(data[8])
        {
            case 0:fprintf(fd,"PSRM:R0 \n");break;
            case 1:fprintf(fd,"PSRM:R1 \n");break;
            case 2:fprintf(fd,"PSRM:R2 \n");break;
            default:;
        }
     }
     else if (GetSuccessor() != 0)
     {
        ParserHandler::HandlePaser(op_code, data, fd);
     }
}

//hci_event_Ox03::hci_event_Ox03(ParserHandler* p, t opcode t):ParserHandler(p, t){}
