/*

Copyright (c) 2018 MacroBull

*/

#pragma once

#include "traits.h"

#include <valarray>

namespace NAMESPACE
{

/*
 * @@@ whitelist is_serializable for:
 *      std::slice_array<TI>, std::gslice_array<TI>,
 *      std::mask_array<TI>, std::indirect_array<TI>,
 *      std::gslice
 *
 */
template <typename TI>
struct is_serializable<std::slice_array<TI>>: is_serializable<TI> {};

template <typename TI>
struct is_serializable<std::gslice_array<TI>>: is_serializable<TI> {};

template <typename TI>
struct is_serializable<std::mask_array<TI>>: is_serializable<TI> {};

template <typename TI>
struct is_serializable<std::indirect_array<TI>>: is_serializable<TI> {};

template <>
struct is_serializable<std::gslice>: std::true_type {};

} // namespace NAMESPACE
