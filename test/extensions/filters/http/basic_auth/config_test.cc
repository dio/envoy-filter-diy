#include "extensions/filters/http/basic_auth/config.h"

#include "test/mocks/server/mocks.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

TEST(BasicAuthConfigFactoryTest, Config) {
  const std::string yaml = R"EOF(
      username: envoy
      password: awesome
      realm: envoy world
    )EOF";

  diy::BasicAuth proto_config;
  MessageUtil::loadFromYaml(yaml, proto_config);

  NiceMock<Server::Configuration::MockFactoryContext> context;
  BasicAuthFilterConfigFactory factory;

  Http::FilterFactoryCb cb = factory.createFilterFactoryFromProto(proto_config, "stats", context);
  Http::MockFilterChainFactoryCallbacks filter_callback;
  EXPECT_CALL(filter_callback, addStreamDecoderFilter(testing::_));
  cb(filter_callback);
}

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy