#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-lambda-capture"
#pragma ide diagnostic ignored "EmptyDeclOrStmt"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "../controller/OTClientController.hpp"

#include "MyControllerTest.hpp"
#include "app/MyApiTestClient.hpp"
#include "app/TestComponent.hpp"

#include "oatpp/core/Types.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"

void MyControllerTest::onRun() {

  /* Register test components */
  TestComponent component;

  /* Create client-server test runner */
  oatpp::test::web::ClientServerTestRunner runner;

  /* Add OTServerController endpoints to the router of the test server */
  runner.addController(std::make_shared<OTClientController>());

  /* Run test */
  runner.run([this, &runner] {

    /* Get client connection provider for Api Client */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvider);

    /* Get object mapper component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

    /* Create http request executor for Api Client */
    auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(clientConnectionProvider);

    /* Create Test API client */
    auto client = MyApiTestClient::createShared(requestExecutor, objectMapper);

    /* Call server API */
    /* Call root endpoint of OTServerController */
    auto response = client->getRoot();

    /* Assert that server responds with 200 */
    OATPP_ASSERT(response->getStatusCode() == 200);

    /* Read response body as MessageDto */
    auto message = response->readBodyToDto<oatpp::Object<ResponseDTO<oatpp::String>>>(objectMapper.get());

    /* Assert that received message is as expected */
    OATPP_ASSERT(message);
    OATPP_ASSERT(message->code == 200);
    OATPP_ASSERT(message->message == "Hello World!");

  }, std::chrono::minutes(10) /* test timeout */);

  /* wait all server threads finished */
  std::this_thread::sleep_for(std::chrono::seconds(1));

}
