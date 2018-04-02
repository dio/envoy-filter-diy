#pragma once

#include "envoy/http/filter.h"

#include "common/common/base64.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

static const std::string UNAUTHORIZED{"unauthorized"};

struct BasicAuthFilterConfig {
  BasicAuthFilterConfig(const std::string& username, const std::string& password,
                        const std::string& realm, const std::string& message = UNAUTHORIZED)
      : encoded_(encode(username, password)), realm_(fmt::format("Basic realm=\"{}\"", realm)),
        message_(message) {}

  const std::string encode(const std::string& username, const std::string& password) {
    const std::string& decoded = fmt::format("{}:{}", username, password);
    return Base64::encode(decoded.c_str(), decoded.size());
  }

  const std::string encoded_;
  const std::string realm_;
  const std::string message_;
};
typedef std::shared_ptr<BasicAuthFilterConfig> BasicAuthFilterConfigPtr;

class BasicAuthFilter : public Http::StreamDecoderFilter {
public:
  BasicAuthFilter(BasicAuthFilterConfigPtr config) : config_(config) {}
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
  BasicAuthFilterConfigPtr config_;
};

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy