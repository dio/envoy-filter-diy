#include "extensions/filters/http/basic_auth/config.h"

#include "test/mocks/server/mocks.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

TEST(BasicAuthConfigFactoryTest, Config) {
  NiceMock<Server::Configuration::MockFactoryContext> context;
  BasicAuthFilterConfigFactory factory;

  Server::Configuration::HttpFilterFactoryCb cb =
      factory.createFilterFactoryFromProto(Envoy::ProtobufWkt::Empty(), "stats", context);

  Http::MockFilterChainFactoryCallbacks filter_callback;
  EXPECT_CALL(filter_callback, addStreamDecoderFilter(testing::_));
  cb(filter_callback);
}

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy