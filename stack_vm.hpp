#ifndef __VM_HPP
#define __VM_HPP

#include "list.hpp"
#include "value.hpp"
#include "control.hpp"
#include "op.hpp"

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

#endif
