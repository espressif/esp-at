#pragma once

#include <vector>
#include "esp_err.h"
#include "esp_partition.h"
#include "Flash_Access.h"

extern std::vector<uint8_t> g_flashmem;

/**
* @brief This class is used to access partition. Class implements Flash_Access interface
*
*/
class FatPartition : public Flash_Access
{

public:
    FatPartition(const esp_partition_t *partition);

    virtual size_t chip_size();

    virtual esp_err_t erase_sector(size_t sector);
    virtual esp_err_t erase_range(size_t start_address, size_t size);

    virtual esp_err_t write(size_t dest_addr, const void *src, size_t size);
    virtual esp_err_t read(size_t src_addr, void *dest, size_t size);

    virtual size_t sector_size();

    virtual ~FatPartition();
protected:
    const esp_partition_t *partition;

};


