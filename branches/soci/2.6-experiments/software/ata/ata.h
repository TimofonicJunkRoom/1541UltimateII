/*
    ATA interface emulation
    Copyright (C) 2015 Kajtar Zsolt (soci@c64.rulez.org)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef ATA_H
#define ATA_H

#include "integer.h"
#include "event.h"

#define ATA_ERR 0x01
#define ATA_DRQ 0x08
#define ATA_DRDY 0x40
#define ATA_BSY 0x80
#define ATA_BUF 0x0F
#define ATA_RST 0x10
#define ATA_DATA 0x20
#define ATA_CMD 0x80

#define ATA_ERROR 1
#define ATA_COUNT 2
#define ATA_SECTOR 3
#define ATA_CYLINDER_LOW 4
#define ATA_CYLINDER_HIGH 5
#define ATA_HEAD 6
#define ATA_COMMAND 7
#define ATA_FEATURE 9
#define ATA_IDNF 0x10
#define ATA_ABRT 0x04
#define ATA_UNC 0x40

class ATA
{
    volatile BYTE *buffer;
    volatile BYTE *registers;
    volatile BYTE *hwregister;
    BYTE cmd, count, pos, fill;
    BYTE write_error;
    int error_lba;
    int lba;
    int write_cache;
    struct {
        int cylinders, heads, sectors, size;
    } geometry;
    struct {
        int cylinders, heads, sectors;
    } actual;
    void diag(void);
    int get_lba(void);
    void set_lba(int lba);
    void set_string(volatile BYTE *b, char *s, int n);
    void calc_geometry(int size);

public:
    ATA(volatile BYTE *regs, volatile BYTE *buf, volatile BYTE *hwreg);
    ~ATA();
    
    void poll(Event &e);
};

#endif

