/*

Copyright (c) 2018 MacroBull

*/

#pragma once

#include "traits.h"

#include <queue>
//#include <stack>

namespace NAMESPACE
{

/*
 * @@@ blacklist is_serializable for:
 *      std::priory_queue<TI, ...>
 *
 */
template <class TI, typename ...Args>
struct is_serialization_copyable_blacklisted<std::priority_queue<TI, Args...>>:
        std::true_type {};

} // namespace NAMESPACE
