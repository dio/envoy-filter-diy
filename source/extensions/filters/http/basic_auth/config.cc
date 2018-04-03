#include "extensions/filters/http/basic_auth/config.h"
#include "extensions/filters/http/basic_auth/basic_auth.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

Server::Configuration::HttpFilterFactoryCb
BasicAuthFilterConfigFactory::createFilterFactoryFromProto(const Protobuf::Message&,
                                                           const std::string&,
                                                           Server::Configuration::FactoryContext&) {
  BasicAuthFilterConfigPtr config = std::make_shared<BasicAuthFilterConfig>(
      BasicAuthFilterConfig("envoy", "awesome", "envoy world"));

  return [config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
    callbacks.addStreamDecoderFilter(
        Http::StreamDecoderFilterSharedPtr{new BasicAuthFilter(config)});
  };
}

static Registry::RegisterFactory<BasicAuthFilterConfigFactory,
                                 Server::Configuration::NamedHttpFilterConfigFactory>
    registered_;

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy