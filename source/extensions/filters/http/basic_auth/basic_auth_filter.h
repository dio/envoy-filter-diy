#pragma once

#include "envoy/http/filter.h"
#include "envoy/runtime/runtime.h"
#include "envoy/stats/scope.h"
#include "envoy/stats/stats_macros.h"

#include "common/common/base64.h"

#include "source/extensions/filters/http/basic_auth/config.pb.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

/**
 * All example filter stats. @see stats_macros.h.
 */
#define ALL_BASIC_AUTH_STATS(COUNTER) COUNTER(called)

/**
 * Struct definition for basic auth stats. @see stats_macros.h.
 */
struct BasicAuthStats {
  ALL_BASIC_AUTH_STATS(GENERATE_COUNTER_STRUCT)
};

static const std::string& UNAUTHORIZED() { CONSTRUCT_ON_FIRST_USE(std::string, "unauthorized"); }

static const Envoy::Http::LowerCaseString& WWW_AUTHENTICATE() {
  CONSTRUCT_ON_FIRST_USE(Envoy::Http::LowerCaseString, "WWW-Authenticate");
}

/**
 * Configuration for the basic auth filter.
 */
class BasicAuthFilterConfig {
public:
  BasicAuthFilterConfig(const diy::BasicAuth& proto, const std::string& stats_prefix,
                        Stats::Scope& scope, Runtime::Loader& runtime)
      : encoded_(encode(proto.username(), proto.password())),
        realm_(fmt::format("Basic realm=\"{}\"", proto.realm())),
        message_(proto.message().empty() ? UNAUTHORIZED() : proto.message()),
        www_authenticate_(WWW_AUTHENTICATE()),
        stats_(generateStats(stats_prefix + "basic_auth.", scope)), runtime_(runtime) {}

  const std::string& encoded() { return encoded_; }
  const std::string& realm() { return realm_; }
  const std::string& message() { return message_; }
  const Envoy::Http::LowerCaseString wwwAuthenticate() { return www_authenticate_; }

  Runtime::Loader& runtime() { return runtime_; }
  BasicAuthStats& stats() { return stats_; }

private:
  const std::string encoded_;
  const std::string realm_;
  const std::string message_;
  const Envoy::Http::LowerCaseString www_authenticate_;

  const std::string encode(const std::string& username, const std::string& password) {
    const std::string& decoded = fmt::format("{}:{}", username, password);
    return Base64::encode(decoded.c_str(), decoded.size());
  }

  static BasicAuthStats generateStats(const std::string& prefix, Stats::Scope& scope) {
    return BasicAuthStats{ALL_BASIC_AUTH_STATS(POOL_COUNTER_PREFIX(scope, prefix))};
  }

  BasicAuthStats stats_;
  Runtime::Loader& runtime_;
};
typedef std::shared_ptr<BasicAuthFilterConfig> BasicAuthFilterConfigSharedPtr;

class BasicAuthFilter : public Http::StreamDecoderFilter {
public:
  BasicAuthFilter(const BasicAuthFilterConfigSharedPtr& config) : config_(config) {}
  ~BasicAuthFilter() {}

  // Http::StreamFilterBase
  void onDestroy() override {}

  // Http::StreamDecoderFilter
  Http::FilterHeadersStatus decodeHeaders(Http::HeaderMap&, bool) override;
  Http::FilterDataStatus decodeData(Buffer::Instance&, bool) override {
    return Http::FilterDataStatus::Continue;
  }
  Http::FilterTrailersStatus decodeTrailers(Http::HeaderMap&) override {
    return Http::FilterTrailersStatus::Continue;
  }
  void setDecoderFilterCallbacks(Http::StreamDecoderFilterCallbacks& callback) override {
    decoder_callbacks_ = &callback;
  }

private:
  bool authenticated(const Http::HeaderMap& headers);

  Http::StreamDecoderFilterCallbacks* decoder_callbacks_;
  BasicAuthFilterConfigSharedPtr config_;
};

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy