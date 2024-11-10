cmake_minimum_required(VERSION 3.12)
project(pg_service_template CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Adding userver dependency
find_package(userver COMPONENTS core postgresql QUIET)
if (NOT userver_FOUND)  # Fallback to subdirectory usage
    # Enable userver libraries that are needed in this project
    set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)

    # Compatibility mode: some systems don't support these features
    set(USERVER_FEATURE_CRYPTOPP_BLAKE2 OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_GRPC_CHANNELZ OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_REDIS_HI_MALLOC ON CACHE BOOL "" FORCE)

    if (EXISTS third_party/userver)
        message(STATUS "Using userver framework from third_party/userver")
        add_subdirectory(third_party/userver)
    else ()
        message(FATAL_ERROR "Either install the userver or provide a path to it")
    endif ()
endif ()

userver_setup_environment()

# Aws dependencies
set(SERVICE_COMPONENTS s3)
find_package(AWSSDK REQUIRED COMPONENTS ${SERVICE_COMPONENTS})

include_directories("/usr/include/libbson-1.0/")
link_directories("/lib/x86_64-linux-gnu/")

pkg_check_modules(LIBBSON REQUIRED libbson-1.0)
include_directories(${LIBBSON_INCLUDE_DIRS})

add_library(${PROJECT_NAME}_service_layer
        src/services/auth/yandex_auth.cpp
        src/services/auth/yandex_auth.hpp
        src/services/auth_service.hpp
        src/services/event/event.cpp
        src/services/event/event.hpp
        src/services/event_service.hpp
        src/services/exception.cpp
        src/services/exception.hpp
        src/services/file_service.hpp
        src/services/file_service/file_service.cpp
        src/services/file_service/file_service.hpp
        src/services/idm/idm.cpp
        src/services/idm/idm.hpp
        src/services/idm/simple_idm_service.cpp
        src/services/idm/simple_idm_service.hpp
        src/services/idm_service.hpp
        src/services/mapping/mapping.cpp
        src/services/mapping/mapping.hpp
        src/services/models/user.cpp
        src/services/models/user.hpp
        src/services/request_management/request_management.cpp
        src/services/request_management/request_management.hpp
        src/services/request_management_service.hpp
        src/services/request_management_service.cpp
)
target_link_libraries(${PROJECT_NAME}_service_layer PUBLIC
        userver::postgresql
        userver::mongo
)
target_include_directories(${PROJECT_NAME}_service_layer PUBLIC src)
target_include_directories(${PROJECT_NAME}_service_layer PUBLIC src/gen)

add_library(${PROJECT_NAME}_data_access_layer
        src/repository/database/event.cpp
        src/repository/database/event.hpp
        src/repository/database/file_meta.cpp
        src/repository/database/file_meta.hpp
        src/repository/database/request.cpp
        src/repository/database/request.hpp
        src/repository/database/user_data.cpp
        src/repository/database/user_data.hpp
        src/repository/event_repository.hpp
        src/repository/exception.h
        src/repository/file_meta_repository.hpp
        src/repository/file_storage_repository.hpp
        src/repository/http/file_storage.cpp
        src/repository/http/file_storage.hpp
        src/repository/http/user_data.cpp
        src/repository/http/user_data.hpp
        src/repository/requests_repository.cpp
        src/repository/requests_repository.hpp
        src/repository/user_data_repository.hpp
        src/repository/database/mongo_request.cpp
        src/repository/database/mongo_request.hpp
)
target_link_libraries(${PROJECT_NAME}_data_access_layer PUBLIC
        userver::postgresql
        userver::mongo
        ${AWSSDK_LINK_LIBRARIES}
)
target_include_directories(${PROJECT_NAME}_data_access_layer PUBLIC src)
target_include_directories(${PROJECT_NAME}_data_access_layer PUBLIC src/gen)

add_library(${PROJECT_NAME}_rest_api_layer
        src/components.cpp
        src/components.hpp
        src/di/init.cpp
        src/di/init.hpp
        src/handlers/base_handler.cpp
        src/handlers/base_handler.hpp
        src/handlers/base_json_handler.hpp
        src/handlers/base_json_handler_fwd.hpp
        src/handlers/v2/event_list_get.cpp
        src/handlers/v2/event_list_get.hpp
        src/handlers/v2/event_post.cpp
        src/handlers/v2/event_post.hpp
        src/handlers/v2/file_put.cpp
        src/handlers/v2/file_put.hpp
        src/handlers/v2/manage_access_put.cpp
        src/handlers/v2/manage_access_put.hpp
        src/handlers/v2/request_comment_post.cpp
        src/handlers/v2/request_comment_post.hpp
        src/handlers/v2/request_get.cpp
        src/handlers/v2/request_get.hpp
        src/handlers/v2/request_list_get.cpp
        src/handlers/v2/request_list_get.hpp
        src/handlers/v2/request_post.cpp
        src/handlers/v2/request_post.hpp
        src/handlers/v3/requests_post.cpp
        src/handlers/v3/requests_get.cpp
        src/handlers/v3/requests_id_get.cpp
        src/handlers/v3/requests_id_comments.cpp
        src/handlers/v3/roles.cpp
        src/handlers/v3/events_get.cpp
        src/handlers/v3/events_post.cpp
        src/handlers/v3/events_id_put.cpp
        src/handlers/v3/files_put.cpp
        src/http/exception.hpp
        src/http/query.cpp
        src/http/query.hpp
        src/http/request.hpp
        src/http/response.hpp
        src/middleware/auth.cpp
        src/middleware/auth.hpp
        src/middleware/auth_pipeline_builder.cpp
        src/middleware/auth_pipeline_builder.hpp
        src/utils/auth.cpp
        src/utils/auth.hpp
        src/utils/json.hpp
        src/utils/map.hpp
        src/utils/merge_utils.hpp
        src/utils/meta.hpp
        src/utils/request.cpp
        src/utils/request.hpp
        src/utils/uuid.cpp
        src/utils/uuid.hpp
        src/repository/factory.hpp
        src/repository/factory.cpp
        src/services/factory.cpp
        src/services/factory.hpp
)
target_link_libraries(
        ${PROJECT_NAME}_rest_api_layer PUBLIC
        userver::postgresql
        userver::mongo
        ${PROJECT_NAME}_service_layer
        ${PROJECT_NAME}_data_access_layer
)
target_include_directories(${PROJECT_NAME}_rest_api_layer PUBLIC src)
target_include_directories(${PROJECT_NAME}_rest_api_layer PUBLIC src/gen)
target_link_libraries(${PROJECT_NAME}_rest_api_layer PUBLIC ${AWSSDK_LINK_LIBRARIES})

include(${CMAKE_CURRENT_SOURCE_DIR}/src/gen/CMakeLists.txt)

# The Service
add_executable(
        ${PROJECT_NAME}
        src/main.cpp
)
target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        ${PROJECT_NAME}_rest_api_layer
        ${PROJECT_NAME}_gen
)

add_executable(${PROJECT_NAME}_unittest
        src/tests/unit/request_object_mother.cpp
        src/tests/unit/request_object_mother.hpp
        src/tests/unit/mock.hpp
        src/tests/unit/base_service_test.cpp
        src/tests/unit/base_service_test.hpp
        src/tests/unit/test_request_service.cpp
        src/tests/unit/test_event_service.cpp
        src/tests/unit/test_file_service.cpp
        src/tests/unit/test_idm_service.cpp
        src/tests/object_mother.hpp
        src/tests/object_mother.cpp
)
target_link_libraries(${PROJECT_NAME}_unittest PRIVATE
        ${PROJECT_NAME}_service_layer
        userver::utest
)
add_google_tests(${PROJECT_NAME}_unittest)

add_executable(${PROJECT_NAME}_inttest
        src/utils/uuid.cpp
        src/utils/uuid.hpp
        src/repository/factory.hpp
        src/repository/factory.cpp
        src/tests/nonmock/test_request_repository.cpp
        src/tests/nonmock/test_request_repository.hpp
        src/tests/nonmock/main.cpp
        src/tests/nonmock/runner.hpp
        src/tests/nonmock/runner.cpp
        src/tests/nonmock/test_component.hpp
        src/tests/nonmock/test_component.cpp
        src/tests/nonmock/test_event_repository.hpp
        src/tests/nonmock/test_event_repository.cpp
        src/tests/nonmock/test_user_data_repository.hpp
        src/tests/nonmock/test_user_data_repository.cpp
        src/tests/object_mother.hpp
        src/tests/object_mother.cpp
)
target_link_libraries(${PROJECT_NAME}_inttest PRIVATE
        userver::mongo
        ${PROJECT_NAME}_data_access_layer
)

# Functional Tests
include(UserverTestsuite)

userver_testsuite_add_simple()

# Install
include(GNUInstallDirs)

if (DEFINED ENV{PREFIX})
    message(STATUS "Set install prefix: $ENV{PREFIX}")
    file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
    set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif ()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)
file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
