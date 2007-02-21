/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_TOKENIZE_H
#define __SHAWN_SYS_MISC_TOKENIZE_H

#include <string>
#include <vector>

namespace shawn
{
    ///String tokenizer helper class
    class StrTok
    {
    public:
        typedef std::vector<std::string> Result;
        typedef std::vector<std::string>::iterator iterator;

        StrTok(const std::string& str, const std::string& delimiters);
        StrTok::iterator begin();
        StrTok::iterator end();
        StrTok::Result::size_type size() const;
        const StrTok::Result tokens() const;

    protected:
        Result res_;
    };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/tokenize.h,v $
 * Version $Revision: 1.4 $
 * Date    $Date: 2006/08/31 20:31:29 $
 *-----------------------------------------------------------------------
 * $Log: tokenize.h,v $
 *-----------------------------------------------------------------------*/
