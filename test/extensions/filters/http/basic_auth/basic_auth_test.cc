#include "extensions/filters/http/basic_auth/basic_auth.h"

#include "test/mocks/http/mocks.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

class BasicAuthFilterTest : public testing::Test {
protected:
  void SetUp() override {
    filter_.reset(new BasicAuthFilter());
    filter_->setDecoderFilterCallbacks(callbacks_);
  }

  testing::NiceMock<Http::MockStreamDecoderFilterCallbacks> callbacks_;
  std::unique_ptr<BasicAuthFilter> filter_;
};

TEST_F(BasicAuthFilterTest, Request) {
  Http::TestHeaderMapImpl request_headers{{":method", "get"}};
  EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(request_headers, false));
}

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy