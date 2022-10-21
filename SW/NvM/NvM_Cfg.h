/* 
 * File:   NvM_Cfg.h
 * Author: tothpetiszilard
 *
 * Created on March 30, 2019, 12:13 PM
 */

#ifndef NVM_CFG_H
#define	NVM_CFG_H

#include "../mcc_generated_files/memory.h"

#define NVM_VALIDMASK_U16                           ((uint16_t)0x2000u)
#define NVM_EMPTYMASK_U16                           ((uint16_t)0x3FFFu)
#define NVM_DATAMASK_U16                            ((uint16_t)0x1FFFu)

#define NVM_MEMREAD(addr_u16)                       FLASH_ReadWord(addr_u16)
#define NVM_WRITE(addr_u16, buffer_pu16, data_u16)  FLASH_WriteWord(addr_u16, buffer_pu16, (data_u16 | NVM_VALIDMASK_U16))
#define NVM_ERASEBLOCK(blockAddr_u16)               FLASH_EraseBlock(blockAddr_u16)
#define NVM_INVALIDATE(addr_u16,buffer_pu16)        FLASH_WriteWord(addr_u16,buffer_pu16,0u)

#define NVM_FLASH_END                               END_FLASH
#define NVM_USED_BLOCKS_CNT                         (63u)
#define NVM_BLOCKSIZE                               WRITE_FLASH_BLOCKSIZE
#define NVM_DATAOFFSET                              (NVM_BLOCKSIZE - 1u)
#define NVM_STARTADDR                               ((NVM_FLASH_END - (NVM_USED_BLOCKS_CNT* NVM_BLOCKSIZE)) + NVM_DATAOFFSET)
#define NVM_CONFIGADDR                              (NVM_FLASH_END - ((NVM_USED_BLOCKS_CNT + 1u)* NVM_BLOCKSIZE))
#define NVM_BLOCK0_ADDR                             (NVM_STARTADDR)
#define NVM_BLOCKN_ADDR(n)                          (NVM_FLASH_END - ( n * WRITE_FLASH_BLOCKSIZE))

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* NVM_CFG_H */

