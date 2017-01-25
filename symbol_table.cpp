#include "symbol_table.h"
#include "astnode.h"

u32 SymbolTable::occupy_size() const
{
  u32 total_size=0;
  for (auto &i : frame_)
  {
    total_size += (i.second)->occupy_size();
  }
  return total_size;
}
