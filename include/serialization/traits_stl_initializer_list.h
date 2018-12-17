/*

Copyright (c) 2018 MacroBull

*/

#pragma once

#include "traits.h"

#include <initializer_list>

namespace NAMESPACE
{

/*
 * @@@ blacklist is_serialization_copyable for:
 *      std::initializer_list<TI>
 *
 */
template <typename TI>
struct is_serialization_copyable_blacklisted<std::initializer_list<TI>>: std::true_type {};

/*
 * @@@ blacklist is_serializable for:
 *      std::initializer_list<TI>
 *
 * std::initializer_list has no public constructor
 *
 */
template <typename TI>
struct is_serializable_blacklisted<std::initializer_list<TI>>: std::true_type {};

//template <class TI>
//struct is_serializable<std::initializer_list<TI>>: is_serializable<TI> {};

} // namespace NAMESPACE
