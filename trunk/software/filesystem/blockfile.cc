
#include <stdio.h>
#include <sys/stat.h>
#include "blockdev.h"

BlockDevice::BlockDevice(char *name)
{
    sector_size = 512;
    struct stat file_status;
    if(!stat(name, &file_status)) {
        file_size = file_status.st_size;
        f = fopen(name, "rb+");
        printf("Construct block device, opening file %s of size %d (%p).\n", name, file_size, f);
    } else {
        f = NULL;
    }
}
    
BlockDevice::~BlockDevice()
{
    if(f)
        fclose(f);
}

DSTATUS BlockDevice::init(void)
{
    // We don't need to do anything here, just return status
    return status();
}
        
DSTATUS BlockDevice::status(void)
{
    if(f)
        return 0;
    else
        return STA_NODISK;
}
    
DRESULT BlockDevice::read(BYTE *buffer, DWORD sector, BYTE count)
{
//    printf("Device read sector %d.\n", sector);

    if(fseek(f, sector * sector_size, SEEK_SET))
        return RES_PARERR;
        
    int read = fread(buffer, sector_size, (int)count, f);
    if(read != (int)count)
        return RES_ERROR;
    
    return RES_OK;
}

#if	_READONLY == 0
DRESULT BlockDevice::write(const BYTE *buffer, DWORD sector, BYTE count)
{
//    printf("Device write sector %d. ", sector);

    if(fseek(f, sector * sector_size, SEEK_SET))
        return RES_PARERR;
        
    int written = fwrite(buffer, sector_size, (int)count, f);
//    printf("%d\n", written);
    if(written != (int)count)
        return RES_ERROR;
    
    return RES_OK;
}
#endif

DRESULT BlockDevice::ioctl(BYTE command, void *data)
{
    DWORD size;
    DWORD *dest = (DWORD *)data;
    
    switch(command) {
        case GET_SECTOR_COUNT:
            if(!f)
                return RES_ERROR;
            size = file_size / sector_size;
            *dest = size;
            break;
        case GET_SECTOR_SIZE:
            (*(DWORD *)data) = sector_size;
            break;
        case GET_BLOCK_SIZE:
            (*(DWORD *)data) = 128*1024;
            break;
        default:
            printf("IOCTL %d.\n", command);
            return RES_PARERR;
    }
    return RES_OK;
}
