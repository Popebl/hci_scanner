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
/*
 * GOF Factory Method Design pattern
 */

//Interface
class ParserFactory
{
    public:
        virtual Parser* CreateParser(void);
};

class ConnectionTimeoutFactory : public ParserFactory
{
    public:
        Parser* CreateParser(void);
};

class ConnectionFactory : public ParserFactory
{
    public:
        Parser* CreateParser(void);
};

class SearchingFactory : public ParserFactory
{
    public:
        Parser* CreateParser(void);
};

class AEperformanceFactory : public ParserFactory
{
    public:
        Parser* CreateParser(void);
};

class SynchronousLinkFactory : public ParserFactory
{
    public:
        Parser* CreateParser(void);
};

class SpecialFactory : public ParserFactory
{
    public:
        Parser* CreateParser(void);
};
