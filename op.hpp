#ifndef __OP_HPP
#define __OP_HPP

enum OP_CODE{
  OP_NOP,
  OP_ADD,
  OP_QUIT,
  OP_INC,
  OP_DUP,
  OP_SWP,
  OP_PUSH_V,
  OP_JEQ,
  OP_JMP,
  OP_GET_STACK,
  OP_DEC_V,
  OP_INC_V,
  OP_ADD_STACK
};

struct NP{};

template<OP_CODE OC, typename PAR = NP>
struct OP
{
  static const OP_CODE Code = OC;
  typedef  PAR Par;
};

#endif
