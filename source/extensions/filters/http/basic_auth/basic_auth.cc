#include "extensions/filters/http/basic_auth/basic_auth.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

Http::FilterHeadersStatus BasicAuthFilter::decodeHeaders(Http::HeaderMap&, bool) {
  return Http::FilterHeadersStatus::Continue;
}

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy