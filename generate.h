#include "memory_map.h"
#include "nvme/nvme.h"

void generateReQ(unsigned int count);
void SIM_H2C_DMA(unsigned int lba , unsigned int databuffer_index);
void SIM_C2H_DMA(unsigned int lba , unsigned int databuffer_index);