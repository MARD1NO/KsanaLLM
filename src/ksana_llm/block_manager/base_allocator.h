/* Copyright 2024 Tencent Inc.  All rights reserved.

==============================================================================*/
#pragma once

#include <vector>
#include "ksana_llm/utils/context.h"
#include "ksana_llm/utils/environment.h"
#include "ksana_llm/utils/id_generator.h"
#include "ksana_llm/utils/status.h"
#include "ksana_llm/block_manager/memory_block.h"

namespace ksana_llm {

// The base class of all allocators.
// All the method must be thread-safe.
class BaseAllocator {
  public:
    BaseAllocator(const AllocatorConfig& allocator_config, std::shared_ptr<Context> context);
    virtual ~BaseAllocator() {}

    // Allocate blocked memory.
    virtual Status AllocateBlocks(size_t block_num, std::vector<int>& blocks);

    // Free blocked memory.
    virtual Status FreeBlocks(const std::vector<int>& blocks);

    // Allocate contiguous memory.
    virtual Status AllocateContiguous(size_t size, int& block_id);

    // Free contiguous memory.
    virtual Status FreeContiguous(int block_id);

    // Get memory address of blocked memory.
    virtual Status GetBlockPtrs(const std::vector<int>& blocks, std::vector<void*>& addrs);

    // Get memory address of contiguous memory.
    virtual Status GetContiguousPtr(int block_id, void*& addr);

    // Get number of free blocked memory.
    virtual size_t GetFreeBlockNumber();

    // Get number of used blocked memory.
    virtual size_t GetUsedBlockNumber();

    // Reset the preallocated blocks.
    Status ResetPreAllocatedBlocks(size_t blocks_num);

    // Check whether contiguous is in used
    bool IsContiguousUsed(const int block_id);

  protected:
    // pre-allocate all blocks.
    void PreAllocateBlocks();

    // Allocate memory.
    virtual void AllocateMemory(void** memory_ptr, size_t bytes) = 0;

    // Free memory.
    virtual void FreeMemory(void* memory_ptr) = 0;

  protected:
    // The global id generator, used for all allocators.
    static IdGenerator id_generator_;

    // The current allocator config.
    AllocatorConfig allocator_config_;

    // The global context.
    std::shared_ptr<Context> context_ = nullptr;

    // The free and used blocks.
    std::unordered_map<int, MemoryBlock> free_blocks_;
    std::unordered_map<int, MemoryBlock> used_blocks_;

    // The used contiguous memory.
    std::unordered_map<int, MemoryBlock> used_contiguous_;

    // Make thread-safe.
    std::mutex block_mutex_;
    std::mutex contiguous_mutex_;
};

}  // namespace ksana_llm
