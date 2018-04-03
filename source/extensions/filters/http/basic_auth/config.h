#pragma once

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

#include "source/extensions/filters/http/basic_auth/config.pb.validate.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace BasicAuth {

static const std::string& BASIC_AUTH_FILTER() {
  CONSTRUCT_ON_FIRST_USE(std::string, "diy.basic_auth");
}

class BasicAuthFilterConfigFactory : public Server::Configuration::NamedHttpFilterConfigFactory {
public:
  Server::Configuration::HttpFilterFactoryCb
  createFilterFactoryFromProto(const Protobuf::Message& proto, const std::string&,
                               Server::Configuration::FactoryContext&) override;

  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new diy::BasicAuth()};
  }

  std::string name() override { return BASIC_AUTH_FILTER(); }

  // Not implemented since this is only required deprecated v1 config.
  Server::Configuration::HttpFilterFactoryCb
  createFilterFactory(const Json::Object&, const std::string&,
                      Server::Configuration::FactoryContext&) override {
    NOT_IMPLEMENTED;
  }
};

} // namespace BasicAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy