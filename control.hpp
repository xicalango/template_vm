#ifndef __CONTROL_HPP
#define __CONTROL_HPP

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

#endif
