#ifndef LIB_DEDUCE_H_
#define LIB_DEDUCE_H_

#include "node.h"

#include <utility>
#include <type_traits>

// This file contains some hacks to deduce at what stage in the assignment we
// are currently in. I.e. are pointers represented using counted_ptr or * and is
// the Node class a template or not?

template <template <typename> typename T>
T<int> get_node_type();

template <typename T>
T get_node_type();

template <typename T>
using Remove_CVRef = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename T>
using Get_Node_Ptr = Remove_CVRef<decltype(*std::declval<T>().begin())>;

// Ugly hacking (see lib/deduce.h) for finding the correct way to initialize and
// handle nodes, depending on what step of the assignment we are currently at
using Node_Type = decltype(get_node_type<Node>());
using Node_Ptr = Get_Node_Ptr<Node_Type>;

#endif
