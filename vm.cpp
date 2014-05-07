#include <iostream>

template<typename H, typename T>
struct L
{
  
  typedef H Head;

  typedef T Tail;

};

struct LE {};

template<typename List>
void print() {
  std::cout << List::Head::Value;
  print<typename List::Tail>();
}

template<>
void print<LE>() {}

template<char c>
struct C
{
  typedef char Type;
  static const char Value = c;
};

template<int i>
struct I
{
  typedef int Type;
  static const int Value = i;
};

template<bool Test, typename Then, typename Else>
struct IF {};

template<typename Then, typename Else>
struct IF<true, Then, Else>
{
  typedef Then Result;
};

template<typename Then, typename Else>
struct IF<false, Then, Else>
{
  typedef Else Result;
};

template<typename List, typename Value>
struct STACK_PUSH
{
  typedef L<Value, List> Result;
};

template<typename List>
struct STACK_POP
{
  typedef typename List::Head Value;
  typedef typename List::Tail Result;
};

template<typename List, int start>
struct SLICE
{
  typedef typename SLICE<typename List::Tail, start-1>::Result Result;
};

template<typename List>
struct SLICE<List, 0>
{
  typedef List Result;
};

enum OP_CODE{
  OP_NOP,
  OP_ADD,
  OP_QUIT,
  OP_INC,
  OP_DUP,
  OP_SWP,
  OP_PUSH_V,
  OP_JEQ,
  OP_JMP
};

struct NP{};

template<OP_CODE OC, typename PAR = NP>
struct OP
{
  static const OP_CODE Code = OC;
  typedef  PAR Par;
};

template<typename P, typename CP, typename S, OP_CODE OC, typename CPAR>
struct VM
{};

template<typename P, typename CP, typename S, typename CPAR>
struct VM<P, CP, S, OP_NOP, CPAR>
{
  typedef typename VM<P, typename CP::Tail, S, CP::Head::Code, typename CP::Head::Par>::Result Result;
};

template<typename P, typename CP, typename S, typename CPAR>
struct VM<P, CP, S, OP_ADD, CPAR>
{
  typedef STACK_POP<S> _OP1;
  typedef STACK_POP<typename _OP1::Result> _OP2;
  typedef STACK_PUSH<typename _OP2::Result, I<_OP1::Value::Value + _OP2::Value::Value> > _RESULT;

  typedef typename VM<P, typename CP::Tail, typename _RESULT::Result, CP::Head::Code, typename CP::Head::Par>::Result Result;
};

template<typename P, typename CP, typename S, typename CPAR>
struct VM<P, CP, S, OP_INC, CPAR>
{
  typedef STACK_POP<S> _OP;
  typedef STACK_PUSH<typename _OP::Result, I<_OP::Value::Value + 1> > _RESULT;

  typedef typename VM<P, typename CP::Tail, typename _RESULT::Result, CP::Head::Code, typename CP::Head::Par>::Result Result;
};

template<typename P, typename CP, typename S, typename CPAR>
struct VM<P, CP, S, OP_DUP, CPAR>
{
  typedef STACK_PUSH<S, typename S::Head> _RESULT;

  typedef typename VM<P, typename CP::Tail, typename _RESULT::Result, CP::Head::Code, typename CP::Head::Par>::Result Result;
};

template<typename P, typename CP, typename S, typename CPAR>
struct VM<P, CP, S, OP_SWP, CPAR>
{
  typedef STACK_POP<S> _TOP;
  typedef STACK_POP<typename _TOP::Result> _TOP2;
  typedef STACK_PUSH<typename _TOP2::Result, typename _TOP::Value> _RES1;
  typedef STACK_PUSH<typename _RES1::Result, typename _TOP2::Value> _RESULT;

  typedef typename VM<P, typename CP::Tail, typename _RESULT::Result, CP::Head::Code, typename CP::Head::Par>::Result Result;
};

template<typename P, typename CP, typename S, typename CPAR>
struct VM<P, CP, S, OP_PUSH_V, CPAR>
{
  typedef typename VM<P, typename CP::Tail, typename STACK_PUSH<S, CPAR>::Result, CP::Head::Code, typename CP::Head::Par>::Result Result;
};

template<typename P, typename CP, typename S, typename CPAR>
struct VM<P, CP, S, OP_JEQ, CPAR>
{
  typedef STACK_POP<S> _OP1;
  typedef STACK_POP<typename _OP1::Result> _OP2;

  typedef typename IF< _OP1::Value::Value == _OP2::Value::Value,
    typename SLICE<P, CPAR::Value>::Result,
    CP>::Result _NEW_CP;


  typedef typename VM<P, typename _NEW_CP::Tail, typename _OP2::Result, _NEW_CP::Head::Code, typename _NEW_CP::Head::Par>::Result Result;
};

template<typename P, typename CP, typename S, typename CPAR>
struct VM<P, CP, S, OP_JMP, CPAR>
{
  typedef typename SLICE<P, CPAR::Value>::Result _NEW_CP;

  typedef typename VM<P, typename _NEW_CP::Tail, S, _NEW_CP::Head::Code, typename _NEW_CP::Head::Par>::Result Result;
};

template<typename P, typename CP, typename S, typename CPAR>
struct VM<P, CP, S, OP_QUIT, CPAR>
{
  typedef S Result;
};

template<typename P>
struct RunVM
{
  typedef typename VM<P, typename P::Tail, LE, P::Head::Code, typename P::Head::Par>::Result::Head Result;
};

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
    L< OP< OP_PUSH_V, I<30> >, //EXP
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

