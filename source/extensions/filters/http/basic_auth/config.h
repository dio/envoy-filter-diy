#pragma once

#include "source/extensions/filters/http/basic_auth/config.pb.validate.h"

#include "extensions/filters/http/common/factory_base.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

static const std::string& BASIC_AUTH_FILTER() {
  CONSTRUCT_ON_FIRST_USE(std::string, "diy.basic_auth");
}

class BasicAuthFilterConfigFactory : public Common::FactoryBase<diy::BasicAuth> {
public:
  BasicAuthFilterConfigFactory() : FactoryBase(BASIC_AUTH_FILTER()) {}

private:
  Http::FilterFactoryCb
  createFilterFactoryFromProtoTyped(const diy::BasicAuth& proto_config,
                                    const std::string& stats_prefix,
                                    Server::Configuration::FactoryContext& context) override;
};

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy