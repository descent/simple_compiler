#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "mytype.h"
#include "env.h"

class SymbolTable : public Environment 
{
  public:
    SymbolTable(const string &name): Environment(0, name)
    {
    }
    // return the symbol table all symbol size
    u32 occupy_size() const;
  private:
    //u16 occupy_size_;
};

#endif
