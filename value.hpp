#ifndef __VALUE_HPP
#define __VALUE_HPP

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

#endif
