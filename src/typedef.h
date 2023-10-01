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
#ifndef C_TYPE_DEF_HEAD
#define C_TYPE_DEF_HEAD

#define CPU_WORD_LENTH 32

#if CPU_WORD_LENTH == 64
typedef unsigned int u_int64;
typedef unsigned short u_int32;
typedef unsigned char u_int8;
typedef int int64;
typedef short int32;
typedef char int8; 
#endif

#if CPU_WORD_LENTH == 32
typedef unsigned long u_int64;
typedef unsigned int u_int32;
typedef unsigned short u_int16;
typedef unsigned char u_int8;
typedef long int64;
typedef int int32;
typedef short int16;
typedef char int8;
#endif

#define TRUE 1
#define FALSE 0

#define TOINT( p ) ((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3])
#define TOINT_UERS( p )  ((p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0])
#define T0SH0RT( p )     ((p[0] << 8) | p[1])
#define LITTLE_ENDIAN_CHAR_TO_SH0RT(p) ((p[1] << 8) | p[0]) 

#define DEBUG 0
#define INFO 1
#define RLEASE 2

#define DEBUG_LEVEL RLEASE
#endif
