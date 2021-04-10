#pragma once

/** @file Utils.hpp
  * @brief A collection of utilities used by libIntegrate
  * @author C.D. Clark III
  * @date 04/06/21
  */

#include<cstddef>
#include<type_traits>


namespace libIntegrate
{
  /**
   * A type that can *only* be implicitly converted to integral types.
   * This can be used to test if a function accepts an int as an argument. If the function
   * accepts a double for example, passing an int literal will cause a implicit conversion
   * to double.
   */
struct IntOnly {
  operator int() {return 1;}
  operator unsigned int() {return 1;}
  operator short int() {return 1;}
  operator unsigned short int() {return 1;}
  operator long int() {return 1;}
  operator unsigned long int() {return 1;}
  operator long long int() {return 1;}
  operator unsigned long long int() {return 1;}
};


template<unsigned P> struct priority: priority<P-1> {};
template<> struct priority<0> {};

template<typename... T>
struct always_false : std::false_type {};

}
