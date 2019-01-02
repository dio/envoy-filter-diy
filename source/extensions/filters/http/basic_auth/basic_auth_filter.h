#pragma once

#include "envoy/http/filter.h"

#include "common/common/base64.h"

#include "source/extensions/filters/http/basic_auth/config.pb.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

static const std::string& UNAUTHORIZED() { CONSTRUCT_ON_FIRST_USE(std::string, "unauthorized"); }

static const Envoy::Http::LowerCaseString& WWW_AUTHENTICATE() {
  CONSTRUCT_ON_FIRST_USE(Envoy::Http::LowerCaseString, "WWW-Authenticate");
}

struct BasicAuthFilterConfig {
  BasicAuthFilterConfig(const diy::BasicAuth& proto)
      : encoded_(encode(proto.username(), proto.password())),
        realm_(fmt::format("Basic realm=\"{}\"", proto.realm())),
        message_(proto.message().empty() ? UNAUTHORIZED() : proto.message()),
        www_authenticate_(WWW_AUTHENTICATE()) {}

  const std::string encode(const std::string& username, const std::string& password) {
    const std::string& decoded = fmt::format("{}:{}", username, password);
    return Base64::encode(decoded.c_str(), decoded.size());
  }

  // TODO(dio): probably it needs to use the string type from protobuf one.
  const std::string encoded_;
  const std::string realm_;
  const std::string message_;
  const Envoy::Http::LowerCaseString www_authenticate_;
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