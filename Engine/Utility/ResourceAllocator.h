#pragma once
#include "Singleton.h"
static constexpr size_t RESOURCE_ALLOC_BUFF_SIZE = 256 * 1024 * 1024; // 256 megabytes should be enough
class ResourceAllocator : public Singleton<ResourceAllocator>
{
public:
  ResourceAllocator();
  ~ResourceAllocator();

  void Clear();

  template<typename T>
  T* Allocate(size_t size)
  {
    T* res = (T*)cur_offset;
    cur_offset += size * sizeof(T);

    assert(cur_offset < (buff + RESOURCE_ALLOC_BUFF_SIZE)
      && "Frame Allocation exceeded buffer size");

    return res;
  }

private:
  unsigned char* buff;
  unsigned char* cur_offset;
};
