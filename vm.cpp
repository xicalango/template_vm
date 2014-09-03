#include <iostream>

#include "stack_vm.hpp"

int main(void) {

  //COMPUTE 33
  typedef 
    L< OP< OP_PUSH_V, I<5> >, 
    L< OP< OP_INC >,
    L< OP< OP_DUP >,
    L< OP< OP_PUSH_V, I<33> >,
    L< OP< OP_JEQ, I<6> >,
    L< OP< OP_JMP, I<1> >,
    L< OP< OP_QUIT >, 
    LE
    > > > > > > > PROG1;

  //COMPUTE (2^EXP) * BASE
  typedef
    L< OP< OP_PUSH_V, I<10> >, //EXP
    L< OP< OP_PUSH_V, I<1> >, //BASE

    //LOOP:
    L< OP< OP_SWP >, 
    L< OP< OP_DUP >, 
    L< OP< OP_PUSH_V, I<0> >, //IF EXP == 0?
    L< OP< OP_JEQ, I<12> >, //GOTO END
    
    L< OP< OP_PUSH_V, I<-1> >,
    L< OP< OP_ADD >, //EXP--

    L< OP< OP_SWP >,
    L< OP< OP_DUP >,
    L< OP< OP_ADD >, //BASE = 2 * BASE;

    L< OP< OP_JMP, I<2> >, //GOTO LOOP

    //END:
    L< OP< OP_SWP >, //SWAP RESULT TO TOP

    L< OP< OP_QUIT >, //QUIT
    LE
    > > > > > > > > > > > > > > PROG2;

  std::cout << RunVM<PROG1>::Result::Value << std::endl;
  std::cout << RunVM<PROG2>::Result::Value << std::endl;

  return 0;

}

