#include "extensions/filters/http/basic_auth/basic_auth_filter.h"

#include "test/mocks/http/mocks.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

class BasicAuthFilterTest : public testing::Test {
protected:
  void SetUp() override {
    // TODO(dio): move this to an separate function.
    const std::string yaml = R"EOF(
      username: envoy
      password: awesome
      realm: envoy world
    )EOF";

    diy::BasicAuth proto_config;
    MessageUtil::loadFromYaml(yaml, proto_config);

    config_.reset(new BasicAuthFilterConfig(proto_config));
    filter_.reset(new BasicAuthFilter(config_));
    filter_->setDecoderFilterCallbacks(callbacks_);
  }

  void testUnauthorizedRequest(Http::TestHeaderMapImpl&& request_headers) {
    Http::TestHeaderMapImpl response_headers{{":status", "401"},
                                             {"content-length", "12"},
                                             {"content-type", "text/plain"},
                                             {"www-authenticate", "Basic realm=\"envoy world\""}};
    EXPECT_CALL(callbacks_, encodeHeaders_(HeaderMapEqualRef(&response_headers), false)).Times(1);
    EXPECT_CALL(callbacks_, encodeData(testing::_, true)).Times(1);

    EXPECT_EQ(Http::FilterHeadersStatus::StopIteration,
              filter_->decodeHeaders(request_headers, false));
  }

  testing::NiceMock<Http::MockStreamDecoderFilterCallbacks> callbacks_;
  std::unique_ptr<BasicAuthFilter> filter_;
  BasicAuthFilterConfigPtr config_;
};

TEST_F(BasicAuthFilterTest, UnauthorizedRequestWithoutAuthorizationHeader) {
  testUnauthorizedRequest(Http::TestHeaderMapImpl{{":method", "get"}});
}

TEST_F(BasicAuthFilterTest, UnauthorizedRequestWithInvalidAuthorizationPrefix) {
  testUnauthorizedRequest(Http::TestHeaderMapImpl{
      {":method", "get"}, {"authorization", "Advance ZW52b3k6YXdlc29tZQ=="}});
}

TEST_F(BasicAuthFilterTest, UnauthorizedRequestWithInvalidAuthorizationValue) {
  testUnauthorizedRequest(
      Http::TestHeaderMapImpl{{":method", "get"}, {"authorization", "Basic INVALID"}});
}

TEST_F(BasicAuthFilterTest, AuthorizedRequest) {
  Http::TestHeaderMapImpl request_headers{{":method", "get"},
                                          // base64 of envoy:awesome is ZW52b3k6YXdlc29tZQ=
                                          {"authorization", "Basic ZW52b3k6YXdlc29tZQ=="}};
  EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(request_headers, false));
}

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy