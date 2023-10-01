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
#include "ParserFactory.h"

Parser* ParserFactory::CreateParser(void)
{
    return 0;
}

Parser* ConnectionTimeoutFactory::CreateParser(void)
{
    return new ConnectionTimeoutParser;
}

Parser* ConnectionFactory::CreateParser(void)
{
    return new ConnectionParser;
}

Parser* SearchingFactory::CreateParser(void)
{
    return new SearchingParser;
}

/*
Parser* AEperformanceFactory::CreateParser(void)
{
    return new AEperfornanceParser;
}

Parser* SynchronousLinkFactory::CreateParser(void)
{
    return new SynchronousLinkParser;
}

Parser* SpecialFactory::CreateParser(void)
{
    return new SpecialParser;
}
*/

