#ifndef INCLUDE_COROUTINE_COMPATIBLE_HPP
#define INCLUDE_COROUTINE_COMPATIBLE_HPP

#define __cpp_impl_coroutine 1
# include <coroutine>
#undef  __cpp_impl_coroutine
namespace std::inline experimental
{
    using std::coroutine_traits;
    using std::coroutine_handle;
}
#endif/*INCLUDE_COROUTINE_COMPATIBLE_HPP*/
