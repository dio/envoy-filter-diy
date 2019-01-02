#include "extensions/filters/http/basic_auth/basic_auth.h"

#include "absl/strings/match.h"
#include "common/http/utility.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

static const std::string& PREFIX() { CONSTRUCT_ON_FIRST_USE(std::string, "Basic "); }

Http::FilterHeadersStatus BasicAuthFilter::decodeHeaders(Http::HeaderMap& headers, bool) {
  if (!authenticated(headers)) {
    Http::Utility::sendLocalReply(
        false,
        [&](Http::HeaderMapPtr&& headers, bool end_stream) -> void {
          headers->addReferenceKey(config_->www_authenticate_, config_->realm_);
          decoder_callbacks_->encodeHeaders(std::move(headers), end_stream);
        },
        [&](Buffer::Instance& data, bool end_stream) -> void {
          decoder_callbacks_->encodeData(data, end_stream);
        },
        false, Http::Code::Unauthorized, config_->message_, absl::nullopt);
    return Http::FilterHeadersStatus::StopIteration;
  }
  return Http::FilterHeadersStatus::Continue;
}

bool BasicAuthFilter::authenticated(const Http::HeaderMap& headers) {
  if (!headers.Authorization()) {
    return false;
  }

  absl::string_view value(headers.Authorization()->value().c_str());
  if (!absl::StartsWith(value, PREFIX())) {
    return false;
  }

  absl::string_view encoded(value.substr(PREFIX().size(), value.size() - PREFIX().size()));
  return config_->encoded_.size() == encoded.size() && absl::StartsWith(config_->encoded_, encoded);
}

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy