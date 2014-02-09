#ifndef VMEM_H
#define VMEM_H

typedef struct
{
  uint32_t magic;  // Magic number, used for error checking and identification.
  uint8_t is_hole; // 1 if this is a hole, 0 if this is a block.
  uint32_t size;   // Size of the block, including this and the footer.
} header_t;

typedef struct
{
  uint32_t magic;     // Magic number, same as in header_t.
  header_t *header; // Pointer to the block header.
} footer_t;

#endif