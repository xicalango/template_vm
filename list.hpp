#ifndef __LIST_HPP
#define __LIST_HPP

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

#endif
