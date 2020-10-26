// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

#pragma once

#include <mutex>

namespace wpi {

#ifdef WPI_HAVE_PRIORITY_MUTEX
using mutex = priority_mutex;
using recursive_mutex = priority_recursive_mutex;
#else
using mutex = ::std::mutex;
using recursive_mutex = ::std::recursive_mutex;
#endif

}  // namespace wpi
