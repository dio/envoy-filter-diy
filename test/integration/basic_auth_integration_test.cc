#include "test/integration/http_integration.h"
#include "test/integration/utility.h"

namespace Envoy {
class BasicAuthFilterIntegrationTest : public HttpIntegrationTest,
                                       public testing::TestWithParam<Network::Address::IpVersion> {

public:
  BasicAuthFilterIntegrationTest()
      : HttpIntegrationTest(Http::CodecClient::Type::HTTP1, GetParam()) {}

  void SetUp() override { initialize(); }

  void initialize() override {
    config_helper_.addFilter(R"EOF(
    name: diy.basic_auth
    config:
    )EOF");
    HttpIntegrationTest::initialize();
  }

protected:
  void testRequest(Http::TestHeaderMapImpl&& request_headers,
                   Http::TestHeaderMapImpl&& expected_response_headers,
                   // we need to wait for upstream response when we sending authorized request.
                   bool wait_for_upstream_response = false) {
    codec_client_ = makeHttpConnection(lookupPort("http"));
    if (wait_for_upstream_response) {
      sendRequestAndWaitForResponse(request_headers, 0, expected_response_headers, 0);
    } else {
      codec_client_->makeHeaderOnlyRequest(request_headers, *response_);
      // and don't wait for upstream response
      response_->waitForEndStream();
    }
    EXPECT_TRUE(response_->complete());
    compareHeaders(response_->headers(), expected_response_headers);
  }

  void compareHeaders(Http::TestHeaderMapImpl&& response_headers,
                      Http::TestHeaderMapImpl& expected_response_headers) {
    response_headers.remove(Envoy::Http::LowerCaseString{"date"});
    response_headers.remove(Envoy::Http::LowerCaseString{"x-envoy-upstream-service-time"});
    EXPECT_EQ(expected_response_headers, response_headers);
  }
};

INSTANTIATE_TEST_CASE_P(IpVersions, BasicAuthFilterIntegrationTest,
                        testing::ValuesIn(TestEnvironment::getIpVersionsForTest()));

TEST_P(BasicAuthFilterIntegrationTest, MissingAuthorizationHeader) {
  testRequest(
      Http::TestHeaderMapImpl{{":method", "GET"}, {":path", "/path"}, {":authority", "host"}},
      Http::TestHeaderMapImpl{{"content-length", "12"},
                              {"content-type", "text/plain"},
                              {"www-authenticate", "Basic realm=\"envoy world\""},
                              {"server", "envoy"},
                              {":status", "401"}});
}

TEST_P(BasicAuthFilterIntegrationTest, InvalidAuthorizationHeaderPrefix) {
  testRequest(Http::TestHeaderMapImpl{{":method", "GET"},
                                      {":path", "/path"},
                                      {":authority", "host"},
                                      {"authorization", "Advance ZW52b3k6YXdlc29tZQ=="}},
              Http::TestHeaderMapImpl{{"content-length", "12"},
                                      {"content-type", "text/plain"},
                                      {"www-authenticate", "Basic realm=\"envoy world\""},
                                      {"server", "envoy"},
                                      {":status", "401"}});
}

TEST_P(BasicAuthFilterIntegrationTest, InvalidAuthorizationHeaderValue) {
  testRequest(Http::TestHeaderMapImpl{{":method", "GET"},
                                      {":path", "/path"},
                                      {":authority", "host"},
                                      {"authorization", "Basic INVALID"}},
              Http::TestHeaderMapImpl{{"content-length", "12"},
                                      {"content-type", "text/plain"},
                                      {"www-authenticate", "Basic realm=\"envoy world\""},
                                      {"server", "envoy"},
                                      {":status", "401"}});
}

TEST_P(BasicAuthFilterIntegrationTest, Authorized) {
  testRequest(
      Http::TestHeaderMapImpl{{":method", "GET"},
                              {":path", "/path"},
                              {":authority", "host"},
                              {"authorization", "Basic ZW52b3k6YXdlc29tZQ=="}},
      Http::TestHeaderMapImpl{{"content-length", "0"}, {"server", "envoy"}, {":status", "200"}},
      true);
}

} // namespace Envoy