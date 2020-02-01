/*
 *  Copyright © 2020 James Larrowe
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SAFE_INTEGER_HPP
# define SAFE_INTEGER_HPP 1

#include <limits>
#include <algorithm>
#include <stdexcept>
#include <type_traits>

template<typename I,
         typename std::enable_if<
                  std::is_integral<I>::value &&
                  std::is_signed<I>::value, bool>::type = true>
class safe_int
{
    I val;

public:
    typedef I value_type;
    static constexpr I max = std::numeric_limits<I>::max();
    static constexpr I min = std::numeric_limits<I>::min();

    safe_int(I i) : val { i } { }

    // allow assigning compatible types
    template<typename N,
             typename std::enable_if<
                      std::is_assignable<N, I>::value, bool>::type = true>
    safe_int(N n = {})
          noexcept(std::is_nothrow_assignable<I, N>::value)
        : val { n }


    template<typename N,
             typename std::enable_if<
                      std::is_assignable<N, I>::value, bool>::type = true>
    safe_int(safe_int<N> n)
          noexcept(std::is_nothrow_assignable<I, N>::value)
        : val { n.value() }

    // operator overloads for other types
    // TODO: Implement

    // access operators
    explicit operator I() const noexcept { return val; }
    I value() const noexcept { return val; }

    safe_int& operator=(I v)
    {
        val = v;
        return *this;
    }

    safe_int operator+()
    {
        return *this;
    }

    safe_int operator-()
    {
        if(val < -max)
            throw std::overflow_error("");

        return safe_int(-val);
    }

    safe_int& operator++()
    {
        if(val == max)
            throw std::overflow_error("");

        ++val;

        return *this;
    }

    safe_int& operator--()
    {
        if(val == min)
            throw std::underflow_error("");

        --val;

        return *this;
    }

    safe_int operator++(int)
    {
        if(val == max)
            throw std::overflow_error("");

        return safe_int(val++);
    }

    safe_int operator--(int)
    {
        if(val == min)
            throw std::underflow_error("");

        return safe_int(val--);
    }

    safe_int& operator+=(I rhs)
    {
        if( rhs > 0 && val > max - rhs )
            throw std::overflow_error("");
        else if( rhs < 0 && val < min - rhs )
            throw std::underflow_error("");

        val += rhs;
        return *this;
    }

    safe_int& operator-=(I rhs)
    {
        if( val >= 0 && rhs < -max )
            throw std::overflow_error("");

        if( rhs < 0 && val > max + rhs )
            throw std::overflow_error("");
        else if( rhs > 0 && val < min + rhs )
            throw std::underflow_error("");

        val -= rhs;
        return *this;
    }

    safe_int& operator*=(I rhs)
    {
        if(val > 0)
        {
            if(rhs > max / val)
                throw std::overflow_error("");
        }
        else if(val < 0)
        {
            if((val < -max || rhs < -max) &&
               (val == -1 || rhs == -1))
            {
                throw std::overflow_error("");
            }
            else if(rhs > min / val)
            {
                throw std::underflow_error("");
            }
        }

        val *= rhs;
        return *this;
    }

    safe_int& operator/=(I rhs)
    {
        if((val < -max || rhs < -max) &&
           (val == -1 || rhs == -1))
        {
            throw std::underflow_error("");
        }

        if(rhs == 0)
            throw std::domain_error("");

        val /= rhs;
        return *this;
    }

    safe_int& operator%=(I rhs)
    {
        if(rhs == 0)
            throw std::domain_error("");

        val %= rhs;
        return *this;
    }

    safe_int operator+(I rhs)
    {
        return safe_int(val) += rhs;
    }

    safe_int operator-(I rhs)
    {
        return safe_int(val) -= rhs;
    }

    safe_int operator*(I rhs)
    {
        return safe_int(val) *= rhs;
    }

    safe_int operator/(I rhs)
    {
        return safe_int(val) /= rhs;
    }

    safe_int operator%(I rhs)
    {
        return safe_int(val) %= rhs;
    }

    safe_int& operator+=(safe_int rhs)
    {
        return *this += rhs.value();
    }

    safe_int& operator-=(safe_int rhs)
    {
        return *this -= rhs.value();
    }

    safe_int& operator*=(safe_int rhs)
    {
        return *this *= rhs.value();
    }

    safe_int& operator/=(safe_int rhs)
    {
        return *this /= rhs.value();
    }

    safe_int& operator%=(safe_int rhs)
    {
        return *this %= rhs.value();
    }

    safe_int operator+(safe_int rhs)
    {
        return safe_int(val) += rhs.value();
    }

    safe_int operator-(safe_int rhs)
    {
        return safe_int(val) -= rhs.value();
    }

    safe_int operator*(safe_int rhs)
    {
        return safe_int(val) *= rhs.value();
    }

    safe_int operator/(safe_int rhs)
    {
        return safe_int(val) /= rhs.value();
    }

    safe_int operator%(safe_int rhs)
    {
        return safe_int(val) %= rhs.value();
    }
};

#endif
