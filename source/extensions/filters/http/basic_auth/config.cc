#include "envoy/registry/registry.h"
#include "source/extensions/filters/http/basic_auth/config.pb.validate.h"

#include "extensions/filters/http/basic_auth/config.h"
#include "extensions/filters/http/basic_auth/basic_auth_filter.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

Http::FilterFactoryCb BasicAuthFilterConfigFactory::createFilterFactoryFromProtoTyped(
    const diy::BasicAuth& proto_config, const std::string& stats_prefix,
    Server::Configuration::FactoryContext& context) {
  BasicAuthFilterConfigSharedPtr config = std::make_shared<BasicAuthFilterConfig>(
      BasicAuthFilterConfig(proto_config, stats_prefix, context.scope(), context.runtime()));

  return [config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
    callbacks.addStreamDecoderFilter(
        Http::StreamDecoderFilterSharedPtr{new BasicAuthFilter(config)});
  };
}

/**
 * Static registration for the basic auth filter. @see NamedHttpFilterConfigFactory.
 */
static Registry::RegisterFactory<BasicAuthFilterConfigFactory,
                                 Server::Configuration::NamedHttpFilterConfigFactory>
    registered_;

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy