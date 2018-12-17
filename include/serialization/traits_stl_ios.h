/*

Copyright (c) 2018 MacroBull

*/

#pragma once

#include "traits.h"

#include <ios>

namespace NAMESPACE
{

/*
 * @@@ whitelist is_serializable for:
 *      std::basic_stringbuf<TC>
 *
 */
template <typename TC>
struct is_serializable<std::basic_stringbuf<TC>>: is_serializable<TC> {};

/*
 * @@@ whitelist is_serializable for:
 *      std::basic_stringstream<TC>
 *
 */
template <typename TC>
struct is_serializable<std::basic_stringstream<TC>>: is_serializable<TC> {};

} // namespace NAMESPACE
