// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

#include <wpi/raw_ostream.h>

namespace wpi {

std::ostream& outs() { return std::cout; }

std::ostream& errs() { return std::cerr; }

}  // namespace wpi
