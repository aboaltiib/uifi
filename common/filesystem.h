/* filesystem.h

Copyright (c) 2015, Nikolaj Schlej. All rights reserved.
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

*/

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "basetypes.h"
#include "ustring.h"
#include "ubytearray.h"
#include <sys/stat.h>
#include <fstream>

#ifdef WIN32
#include <direct.h>
static inline bool isExistOnFs(const UString & path) {
    struct _stat buf;
    return (_stat(path.toLocal8Bit(), &buf) == 0);
}

static inline bool makeDirectory(const UString & dir) {
    return (_mkdir(dir.toLocal8Bit()) == 0);
}

static inline bool changeDirectory(const UString & dir) {
    return (_chdir(dir.toLocal8Bit()) == 0);
}
#else
#include <unistd.h>
static inline bool isExistOnFs(const UString & path) {
    struct stat buf;
    return (stat(path.toLocal8Bit(), &buf) == 0);
}

static inline bool makeDirectory(const UString & dir) {
    return (mkdir(dir.toLocal8Bit(), ACCESSPERMS) == 0);
}

static inline bool changeDirectory(const UString & dir) {
    return (chdir(dir.toLocal8Bit()) == 0);
}
#endif

static inline USTATUS readFileIntoArray(const UString & inPath, UByteArray &array) {
    if (!isExistOnFs(inPath))
        return U_FILE_OPEN;

    std::ifstream inputFile(inPath.toLocal8Bit(), std::ios::in | std::ios::binary);
    if (!inputFile)
        return U_FILE_OPEN;
    std::vector<char> buffer(std::istreambuf_iterator<char>(inputFile),
        (std::istreambuf_iterator<char>()));
    inputFile.close();

    array = buffer;

    return U_SUCCESS;
}

#endif
