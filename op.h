#ifndef OP_H
#define OP_H

//#include <map>

class Precedence
{
  public:
    Precedence(){}
    Precedence(int value, bool a): value_(value), left_assoc_(a)
    {
    }
    int value_;
    bool left_assoc_;
  private:
};

#if 0
class OpPrecedenceParser
{
  public:
    
  private:
};
#endif

#endif
