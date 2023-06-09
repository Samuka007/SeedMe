#ifndef MyController_hpp
#define MyController_hpp

#include "dto/DTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include "database/SeedDB.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin Codegen

/**
 * Sample Api Controller.
 */
class MyController : public oatpp::web::server::api::ApiController, public seeddb::SeedDB {
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, apiObjectMapper))
    : oatpp::web::server::api::ApiController(apiObjectMapper), seeddb::SeedDB()
  {}
public:
  
  ENDPOINT("GET", "/", root) {
    auto dto = MessageDto::createShared();
    dto->statusCode = 200;
    dto->message = "Hello World!";
    return createDtoResponse(Status::CODE_200, dto);
  }
  
  // TODO Insert Your endpoints here !!!

  ENDPOINT("GET", "/api", root) {
    auto dto = SrcDto::createShared();
    dto->srcID
  }
  
  ENDPOINT("GET", "/users", getUsers,
         QUERY(Int32, age)) {
    //OATPP_LOGD("Test", "age=%d", age->getValue());
    return createResponse(Status::CODE_200, "OK");
  }

  ENDPOINT("GET", "/users/{userId}", userMessage,
         PATH(Int64, userId)) {
    //OATPP_LOGD("Test", "userId=%d", userId->getValue());
    //Now here can use userID->getValue()
    auto usrdto = UserDto::createShared();
    usrdto->userId = userId.getValue();
    usrdto->userName = 
    return createResponse(Status::CODE_200, "OK");
  }

};

#include OATPP_CODEGEN_END(ApiController) ///< End Codegen

#endif /* MyController_hpp */