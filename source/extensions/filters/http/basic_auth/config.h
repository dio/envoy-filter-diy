#pragma once

#include "source/extensions/filters/http/basic_auth/config.pb.h"
#include "source/extensions/filters/http/basic_auth/config.pb.validate.h"

#include "extensions/filters/http/common/factory_base.h"
#include "extensions/filters/http/extended_well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

/**
 * Config registration for the basic auth filter. @see NamedHttpFilterConfigFactory.
 */
class BasicAuthFilterConfigFactory : public Common::FactoryBase<diy::BasicAuth> {
public:
  BasicAuthFilterConfigFactory() : FactoryBase(ExtendedHttpFilterNames::get().BasicAuth) {}

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