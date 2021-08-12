#include "generate.h"
#include "nvme/host_lld.h"
void generateReQ(unsigned int count)
{
	NVME_IO_COMMAND * nvmeIOCmd_ptr;
    nvmeIOCmd_ptr = (NVME_IO_COMMAND *)(NVME_REQ_SIM_ADDR + (count-1)*sizeof(NVME_COMMAND));
    char * char_addr_ptr;
    if(count%2==1)
    {
    	nvmeIOCmd_ptr->OPC=IO_NVM_WRITE;
    }
    else
    {
    	nvmeIOCmd_ptr->OPC=IO_NVM_READ;
    }
    nvmeIOCmd_ptr->dword[10]=0x12340000 + (count-1) * BYTES_PER_DATA_REGION_OF_SLICE;
    nvmeIOCmd_ptr->dword[11]=0x00000000;
    //?????????????????????????
    nvmeIOCmd_ptr->dword[12]=0x00010000;

    unsigned int addr_incr;
    addr_incr=0;
    unsigned int i;
    char_addr_ptr = (char*)(NVME_DATA_SIM_ADDR + (count-1) * BYTES_PER_DATA_REGION_OF_SLICE);
    for(i=0 ; i<BYTES_PER_DATA_REGION_OF_SLICE ; i++)
    {
    	*(char_addr_ptr+i) = (char) count;
    }
}
void SIM_H2C_DMA( unsigned int lba , unsigned int databuffer_index)
{
	unsigned int count;
	count = (lba - 0x12340000) / BYTES_PER_DATA_REGION_OF_SLICE + 1;
    unsigned int i;
    char * char_addr_ptr;
    char * databuffer_ptr;
    char_addr_ptr = (char*)(NVME_DATA_SIM_ADDR + (count-1) * BYTES_PER_DATA_REGION_OF_SLICE);
    databuffer_ptr = (char*)(DATA_BUFFER_BASE_ADDR + databuffer_index * BYTES_PER_DATA_REGION_OF_SLICE);
    for(i=0 ; i<BYTES_PER_DATA_REGION_OF_SLICE ; i++)
    {
    	*(databuffer_ptr+i) = *(char_addr_ptr+i);
    }
    g_hostDmaStatus.fifoTail.autoDmaRx++;
    g_hostDmaStatus.autoDmaRxCnt++;
}

void SIM_C2H_DMA(unsigned int lba , unsigned int databuffer_index)
{
	unsigned int count;
	count = (lba - 0x12340000) / BYTES_PER_DATA_REGION_OF_SLICE + 1;
    unsigned int i;
    char * char_addr_ptr;
    char * databuffer_ptr;
    char_addr_ptr = (char*)(NVME_DATA_SIM_ADDR + (count-1) * BYTES_PER_DATA_REGION_OF_SLICE);
    databuffer_ptr = (char*)(DATA_BUFFER_BASE_ADDR + databuffer_index * BYTES_PER_DATA_REGION_OF_SLICE);
    for(i=0 ; i<BYTES_PER_DATA_REGION_OF_SLICE ; i++)
    {
    	*(char_addr_ptr+i) = *(databuffer_ptr+i);
    }
    g_hostDmaStatus.fifoTail.autoDmaTx++;
    g_hostDmaStatus.autoDmaTxCnt++;
}
