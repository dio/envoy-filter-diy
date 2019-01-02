#pragma once

#include "common/config/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {

/**
 * Extended well-known http filter names.
 */
class ExtendedHttpFilterNameValues {
public:
  // BasicAuth filter
  const std::string BasicAuth = "diy.basic_auth";
};

typedef ConstSingleton<ExtendedHttpFilterNameValues> ExtendedHttpFilterNames;

} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy