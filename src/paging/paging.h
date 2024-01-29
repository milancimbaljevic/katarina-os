#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "memory/memory.h"

#define PAGING_CACHE_DISABLED 0b00010000
#define PAGING_WRITE_THROUGH 0b00001000
#define PAGING_ACCESS_FROM_ALL 0b00000100
#define PAGING_IS_WRITEABLE 0b00000010
#define PAGING_IS_PRESENT 0b00000001

extern "C" void paging_load_directory(uint32_t *directory);
extern "C" void enable_paging();

#define PAGE_SIZE 4096
#define LEVEL_1_PMT_NUM_OF_ENTRIES 1024
#define LEVEL_2_PMT_NUM_OF_ENTRIES 1024

class PageMapTable
{
protected:
    uint32_t *level1_pmt = nullptr;

public:
    virtual void init(uint8_t flags = PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT | PAGING_IS_WRITEABLE) = 0;
    bool is_paging_aligned(void *addr) { return (((uint32_t)addr % PAGE_SIZE) == 0); }
    void load_me_into_pmtr()
    {
        paging_load_directory(level1_pmt);
        current_page_map_table = this;
    }

    uint32_t get_level1_index(void *virtual_addres)
    {
        return ((uint32_t)virtual_addres) >> 22;
    };

    uint32_t get_level2_index(void *virtual_address)
    {
        return ((uint32_t)virtual_address << 10) >> 22;
    }

    bool map_virtual_to_physical_address(void *virtual_address, uint32_t frame_number_with_flags)
    {
        if (!is_paging_aligned(virtual_address))
            return false;

        uint32_t level1_index = get_level1_index(virtual_address);
        uint32_t level2_index = get_level2_index(virtual_address);

        uint32_t *level2_pmt = (uint32_t *)(level1_pmt[level1_index] & 0xfffff000);
        level2_pmt[level2_index] = frame_number_with_flags;

        return true;
    }

    static PageMapTable *current_page_map_table;
    virtual ~PageMapTable() {}
};

class OneToOnePMT : public PageMapTable
{
public:
    void init(uint8_t flags = PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT | PAGING_IS_WRITEABLE) override
    {
        level1_pmt = (uint32_t *)malloc(sizeof(uint32_t) * LEVEL_1_PMT_NUM_OF_ENTRIES);
        int offset = 0;
        for (int i = 0; i < LEVEL_1_PMT_NUM_OF_ENTRIES; i++)
        {
            uint32_t *level2_pmt = (uint32_t *)malloc(sizeof(uint32_t) * LEVEL_2_PMT_NUM_OF_ENTRIES);
            for (int j = 0; j < LEVEL_2_PMT_NUM_OF_ENTRIES; j++)
            {
                level2_pmt[j] = (offset + (j * PAGE_SIZE)) | flags;
            }
            offset += PAGE_SIZE * LEVEL_1_PMT_NUM_OF_ENTRIES;
            level1_pmt[i] = (uint32_t)level2_pmt | flags | PAGING_IS_WRITEABLE;
        }
    }

    ~OneToOnePMT() {}
};