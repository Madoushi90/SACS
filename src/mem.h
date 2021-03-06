#pragma once
#include <stdint.h>

typedef enum { MEM_FAKE_ALLOC=1, MEM_USE_LOCKED=2 } mem_alloc_flags;

int mem_init(void);
int mem_cleanup(void);

uint32_t mem_dynamic_alloc(uint32_t size, mem_alloc_flags flags);
int mem_free(uint32_t loc);

void* mem_translate_addr(uint32_t loc);

uint8_t  mem_read8(uint32_t loc);
uint16_t mem_read16(uint32_t loc);
uint32_t mem_read32(uint32_t loc);

uint8_t mem_write8(uint32_t loc, uint8_t data);
uint16_t mem_write16(uint32_t loc, uint16_t data);
uint32_t mem_write32(uint32_t loc, uint32_t data);
