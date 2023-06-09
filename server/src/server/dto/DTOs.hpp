#ifndef DTOs_hpp
#define DTOs_hpp

#include "oatpp/core/data/mapping/type/Object.hpp"
#include "oatpp/core/macro/codegen.hpp"

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Message Data-Transfer-Object
 */
class MessageDto : public oatpp::DTO {

  DTO_INIT(MessageDto, DTO /* Extends */)

  DTO_FIELD(Int32, statusCode);   // Status code field
  DTO_FIELD(String, message);     // Message field

};

/* TODO - Add more DTOs here */

class UserDto : public oatpp::DTO {
  DTO_INIT(UserDto, DTO)

  DTO_FIELD(Int32, userId);
  DTO_FIELD(String, userName);
  DTO_FIELD(String, userPW);
};

class SrcDto : public oatpp::DTO {
  DTO_INIT(SrcDto, DTO)

  DTO_FIELD(oatpp::Fields<Int32, String>, SrcName);
  DTO_FIELD(oatpp::Fields<Int32, String>, Magnet);
};

class OperDto : public oatpp::DTO {
  DTO_INIT(OperDto, DTO)

  DTO_FIELD(Int32, operation);
};

/**
 * - User Operate Api
 *  - Sign up
 *  - Edit
 *  - Sign in
 *  - Sign out
 *  - Delete
 * - Data Operate Api
 *  - Sign src
 *  - Get src
 *  - Edit src
 *  - Delete src
*/

/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */