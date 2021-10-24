#pragma once

#include "../Utils.hpp"

/** @file Utils.hpp
  * @brief 
  * @author C.D. Clark III
  * @date 04/06/21
  */


namespace libIntegrate {
  namespace detail {
    template<typename C>
    auto getElement(const C& c, std::size_t i, priority<1>) -> decltype(c.COULD_NOT_DETECT_METHOD_FOR_INDEXING_CONTAINER)
    {
      static_assert( always_false<C>::value, "Could not detect a method for indexing container. The container must provide either operator[] or operator() that take a single integral arguemnt." );
    }

    template<typename C>
    auto getElement(const C& c, std::size_t i, priority<2>) -> decltype(c(IntOnly{}))
    {
      return c(i);
    }

    template<typename C>
    auto getElement(const C& c, std::size_t i, priority<3>) -> decltype(c[IntOnly{}])
    {
      return c[i];
    }


    template<typename C>
    auto getSize(const C& c, priority<1>) -> decltype(c.COULD_NOT_DETECT_METHOD_FOR_GETTING_CONTAINER_SIZE)
    {
      static_assert( always_false<C>::value, "Could not detect a method for obtaining the size of the container. The container must provide either .size(), .length(), or .rows()." );
    }

    template<typename C>
    auto getSize(const C& c, priority<2>) -> decltype(c.size())
    {
      return c.size();
    }

    template<typename C>
    auto getSize(const C& c, priority<2>) -> decltype(c.length())
    {
      return c.length();
    }

    template<typename C>
    auto getSize(const C& c, priority<2>) -> decltype(c.rows())
    {
      return c.rows();
    }

  }




    /**
     * A function template for accessing the i'th element of a container. This function
     * will test if the container provides operator[](int) or operator()(int). If
     * both are provided, the subscript operator is preferred.
     *
     * If neither function is provided, it will cause a compiler error.
     */
    template<typename C>
    auto getElement(const C& c, std::size_t i) -> decltype( detail::getElement(c, i, priority<10>{}) )
    {
      return detail::getElement(c, i, priority<10>{});
    }

    /**
     * A function template for getting the size of a container. This function
     * will test if various, common methods for providing the container size are provided.
     */
    template<typename C>
    auto getSize(const C& c) -> decltype( detail::getSize(c, priority<10>{}) )
    {
      return detail::getSize(c, priority<10>{});
    }
    
}

