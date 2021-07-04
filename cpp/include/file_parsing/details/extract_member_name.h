#pragma once

/** \addtogroup file_parsing
 * @{
 */

#include "expected.h"

#include <string>

Expected<std::string, std::string>
extractMemberName(std::string const& memberDefinition);

/** @}*/