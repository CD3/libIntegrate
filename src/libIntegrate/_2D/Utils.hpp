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
    auto getElement(const C& c, std::size_t i, std::size_t j, priority<1>) -> decltype(c.COULD_NOT_DETECT_METHOD_FOR_INDEXING_CONTAINER)
    {
      static_assert( always_false<C>::value, "Could not detect a method for indexing container. The container must provide either operator[] or operator() that take a single integral arguemnt." );
    }

    template<typename C>
    auto getElement(const C& c, std::size_t i, std::size_t j, priority<2>) -> decltype(c[IntOnly{}][IntOnly{}])
    {
      return c[i][j];
    }

    template<typename C>
    auto getElement(const C& c, std::size_t i, std::size_t j, priority<3>) -> decltype(c(IntOnly{},IntOnly{}))
    {
      return c(i,j);
    }



    template<typename C>
    auto getSizeX(const C& c, priority<1>) -> decltype(c.COULD_NOT_DETECT_METHOD_FOR_GETTING_CONTAINER_SIZE)
    {
      static_assert( always_false<C>::value, "Could not detect a method for obtaining the size along the first index of the of the container. The container must provide either .size(int), .length(int), or .rows()." );
    }

    template<typename C>
    auto getSizeX(const C& c, priority<2>) -> decltype(c[0].size(),c.size())
    {
      return c.size();
    }

    template<typename C>
    auto getSizeX(const C& c, priority<3>) -> decltype(c.size(0))
    {
      return c.size(0);
    }

    template<typename C>
    auto getSizeX(const C& c, priority<4>) -> decltype(c.length(0))
    {
      return c.length(0);
    }

    template<typename C>
    auto getSizeX(const C& c, priority<5>) -> decltype(c.rows())
    {
      return c.rows();
    }


    template<typename C>
    auto getSizeY(const C& c, priority<1>) -> decltype(c.COULD_NOT_DETECT_METHOD_FOR_GETTING_CONTAINER_SIZE)
    {
      static_assert( always_false<C>::value, "Could not detect a method for obtaining the size along the second index of the of the container. The container must provide either .size(int), .length(int), or .cols()." );
    }

    template<typename C>
    auto getSizeY(const C& c, priority<2>) -> decltype(c[0].size(),c.size())
    {
      return c[0].size();
    }

    template<typename C>
    auto getSizeY(const C& c, priority<3>) -> decltype(c.length(0))
    {
      return c.length(1);
    }

    template<typename C>
    auto getSizeY(const C& c, priority<4>) -> decltype(c.size(0))
    {
      return c.size(1);
    }

    template<typename C>
    auto getSizeY(const C& c, priority<5>) -> decltype(c.cols())
    {
      return c.cols();
    }

  }




    /**
     * A function template for accessing the (i,j)'th element of a 2D container. This function
     * will test if the container provides operator[](int) or operator()(int). If
     * both are provided, the subscript operator is preferred.
     *
     * If neither function is provided, it will cause a compiler error.
     */
    template<typename C>
    auto getElement(const C& c, std::size_t i, std::size_t j) -> decltype( detail::getElement(c, i, j, priority<10>{}) )
    {
      return detail::getElement(c, i, j, priority<10>{});
    }

    /**
     * A function template for getting the size of a 2D container along the first index direction. This function
     * will test if various, common methods for providing the container size are provided.
     */
    template<typename C>
    auto getSizeX(const C& c) -> decltype( detail::getSizeX(c, priority<10>{}) )
    {
      return detail::getSizeX(c, priority<10>{});
    }

    /**
     * A function template for getting the size of a 2D container along the second index direction. This function
     * will test if various, common methods for providing the container size are provided.
     */
    template<typename C>
    auto getSizeY(const C& c) -> decltype( detail::getSizeY(c, priority<10>{}) )
    {
      return detail::getSizeY(c, priority<10>{});
    }
    
}

