#include "NvM.h"
#include "NvM_Cfg.h"

static uint16_t NvM_ramData_u16 = 1u;
static uint16_t NvM_flashAddr_u16 = ((NVM_FLASH_END - NVM_BLOCKSIZE) + NVM_DATAOFFSET);

static uint16_t NvM_buffer_au16[NVM_BLOCKSIZE];

static void NvM_ManageBlocks(void);

void NvM_Init(void)
{
    uint16_t tmpData_u16 = 0u;
    NvM_flashAddr_u16 = ((NVM_FLASH_END - NVM_BLOCKSIZE) + NVM_DATAOFFSET);
    tmpData_u16 = NVM_MEMREAD(NvM_flashAddr_u16);
    while((0u == (NVM_VALIDMASK_U16 & tmpData_u16)) || (NVM_EMPTYMASK_U16 == (NVM_EMPTYMASK_U16 & tmpData_u16)))
    {
        if (NVM_STARTADDR <=  NvM_flashAddr_u16)
        {
            if (NVM_STARTADDR !=  NvM_flashAddr_u16)
            {
                NvM_flashAddr_u16 -= NVM_BLOCKSIZE;
                tmpData_u16 = NVM_MEMREAD(NvM_flashAddr_u16);
            }
            else
            {
                tmpData_u16 = NVM_MEMREAD(NvM_flashAddr_u16);
                break;
            }
        }
        else
        {
            /* No valid NvM data found */
            NvM_flashAddr_u16 = NVM_STARTADDR;
            break;
        }
    }
    if ((0u != (NVM_VALIDMASK_U16 & tmpData_u16)) && (NVM_EMPTYMASK_U16 != tmpData_u16))
    {
        /* NvM valid block found */
        NvM_ramData_u16 = (tmpData_u16 & NVM_DATAMASK_U16);
        NvM_ManageBlocks();
    }
}

static void NvM_ManageBlocks(void)
{
    uint8_t tmp_u8 = 1u;
    if (NvM_flashAddr_u16 == (NVM_FLASH_END - NVM_BLOCKSIZE))
    {
        /* Erase oldest data */
        NVM_ERASEBLOCK(NVM_BLOCK0_ADDR);
        /* Write current data into first word */
        NVM_WRITE(NVM_STARTADDR, NvM_buffer_au16, NvM_ramData_u16);
        /* Delete newer data */
        for(tmp_u8 = 1u;tmp_u8 < (NVM_USED_BLOCKS_CNT - 1u);tmp_u8++)
        {
            NVM_ERASEBLOCK(NVM_BLOCKN_ADDR(tmp_u8));
        }
        
    }
}

uint8_t NvM_ReadCfg(void)
{
    uint16_t rawData_u16 = NVM_MEMREAD(NVM_CONFIGADDR);
    if (rawData_u16 == NVM_EMPTYMASK_U16)
    {
       rawData_u16 = 99u; 
    }
    return (uint8_t)rawData_u16;
}

uint16_t NvM_Read(void)
{
    return NvM_ramData_u16;
}

void NvM_WriteCfg(uint8_t cfgData_u8)
{
    NVM_ERASEBLOCK(NVM_CONFIGADDR);
    NVM_WRITE(NVM_CONFIGADDR,NvM_buffer_au16,(uint16_t)cfgData_u8);
}

void NvM_Write(uint16_t data_u16)
{
    if (NvM_ramData_u16 != data_u16)
    {
        NvM_ramData_u16 = data_u16;
        NvM_flashAddr_u16 += NVM_BLOCKSIZE;
        NVM_WRITE(NvM_flashAddr_u16,NvM_buffer_au16, NvM_ramData_u16);
        NvM_ManageBlocks();
    }
}