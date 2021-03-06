﻿/***********************************************************************************
 * Copyright (c) 2013, baickl(baickl@gmail.com)
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 * 
 * * Neither the name of the {organization} nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************/
#ifndef _VFS_PAK_PRIVATE_H_
#define _VFS_PAK_PRIVATE_H_

#include <vfs/base.h>
#include "hashtable/hashtable.h"
#include "hashtable/hashtable_itr.h"
#include "pool.h"

#define PAK_VERSION         1
#define PAK_MAX_PLUGIN_LEN  32

typedef struct pak_header_s
{
	VFS_INT32			_M_flag;
	VFS_INT32			_M_version;
	VFS_INT32			_M_count;
	VFS_UINT64			_M_offset;
}pak_header;

static const VFS_INT32 pak_header_size = sizeof(VFS_INT32)+ 
									     sizeof(VFS_INT32)+ 
									     sizeof(VFS_INT32)+
									     sizeof(VFS_UINT64);

typedef struct pak_iteminfo_s
{
	VFS_UINT64			_M_offset;
	
	VFS_UINT64			_M_size;
	VFS_UINT32			_M_crc32;

    VFS_UINT64			_M_compress_size;
	VFS_CHAR			_M_compress_plugin[PAK_MAX_PLUGIN_LEN+1];
	
}pak_iteminfo;

typedef struct pak
{
	VFS_CHAR			_M_filename[VFS_MAX_FILENAME+1];
	pak_header		    _M_header;
    struct hashtable   *_M_ht_iteminfos;
}pak;

static unsigned int pak_item_hashcode(VFS_VOID *k)  
{  
    const VFS_BYTE *name = (const VFS_BYTE *)k;  
    unsigned long h = 0, g;  
    int i; 
    int len;

    len = strlen((const VFS_CHAR*)k);
  
    for(i=0;i<len;i++)  
    {  
        h = (h << 4) + (unsigned long)(name[i]); 
        if ((g = (h & 0xF0000000UL))!=0)       
            h ^= (g >> 24);  
        h &= ~g;  
    }  


    return (unsigned int)h;  
}

static int pak_item_equalkeys(VFS_VOID *k1, VFS_VOID *k2)
{
    VFS_CHAR  *_k1,*_k2;

    _k1 = (VFS_CHAR*)k1;
    _k2 = (VFS_CHAR*)k2;

    return (0 == strcmp(_k1,_k2));
}

static VFS_VOID pak_item_key_free(VFS_VOID*key)
{
    if( key )
    {
        vfs_pool_free(key);
    }
}

static VFS_VOID pak_item_value_free(VFS_VOID*key)
{
    if( key )
    {
        vfs_pool_free(key);
    }
}

DEFINE_HASHTABLE_INSERT(pak_item_insert, VFS_CHAR, pak_iteminfo)
DEFINE_HASHTABLE_SEARCH(pak_item_search, VFS_CHAR, pak_iteminfo)
DEFINE_HASHTABLE_REMOVE(pak_item_remove, VFS_CHAR, pak_iteminfo)
DEFINE_HASHTABLE_ITERATOR_SEARCH(pak_item_iter_search, VFS_CHAR)

#endif/*_VFS_PAK_PRIVATE_H_*/
