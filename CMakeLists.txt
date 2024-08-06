cmake_minimum_required(VERSION 3.12)
project(pg_service_template CXX)


# Adding userver dependency
find_package(userver COMPONENTS core postgresql QUIET)
if(NOT userver_FOUND)  # Fallback to subdirectory usage
    # Enable userver libraries that are needed in this project
    set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)

    # Compatibility mode: some systems don't support these features
    set(USERVER_FEATURE_CRYPTOPP_BLAKE2 OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_GRPC_CHANNELZ OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_REDIS_HI_MALLOC ON CACHE BOOL "" FORCE)

    if (EXISTS third_party/userver)
        message(STATUS "Using userver framework from third_party/userver")
        add_subdirectory(third_party/userver)
    else()
        message(FATAL_ERROR "Either install the userver or provide a path to it")
    endif()
endif()

userver_setup_environment()


# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
    src/components.hpp
    src/components.cpp
    src/handlers/v1/request_post.hpp
    src/handlers/v1/request_post.cpp
    src/services/auth_service.hpp
    src/repository/user_data_repository.hpp
    src/repository/database/user_data.hpp
    src/services/auth/yandex_auth.hpp
    src/services/auth/yandex_auth.cpp
    src/repository/database/user_data.cpp
    src/repository/http/user_data.hpp
    src/repository/http/user_data.cpp
    src/handlers/v1/base_handler.hpp
    src/handlers/v1/base_handler.cpp
    src/handlers/v1/manage_access_post.hpp
    src/handlers/v1/manage_access_post.cpp
    src/services/idm_service.hpp
    src/services/models/permission.hpp
    src/services/idm/idm.hpp
    src/services/idm/idm.cpp
    src/services/mapping/mapping.cpp
    src/services/mapping/mapping.hpp
    src/services/exception.hpp
    src/utils/merge_utils.hpp
    src/services/models/permission.cpp
    src/repository/requests_repository.hpp
    src/repository/requests_repository.cpp
    src/repository/database/request.hpp
    src/repository/database/request.cpp
    src/services/request_management_service.hpp
    src/services/request_management/request_management.hpp
    src/services/factory.hpp
    src/services/factory.cpp
    src/repository/factory.cpp
    src/repository/factory.hpp
    src/services/request_management/request_management.cpp
    src/services/file_service.hpp
    src/services/file_service/file_service.cpp
    src/services/file_service/file_service.hpp
    src/repository/file_storage_repository.hpp
    src/repository/file_meta_repository.hpp
    src/repository/database/file_meta.cpp
    src/repository/database/file_meta.hpp
    src/repository/http/file_storage.cpp
    src/repository/http/file_storage.hpp
    src/services/exception.cpp
    src/utils/uuid.hpp
    src/utils/uuid.cpp
    src/handlers/v2/handler.hpp
    src/http/request.hpp
    src/http/response.hpp
    src/handlers/v2/request_post.hpp
    src/handlers/v2/request_post.cpp
    src/di/init.hpp
    src/di/init.cpp
    src/utils/auth.hpp
        src/http/exception.hpp
)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver::postgresql)
target_include_directories(${PROJECT_NAME}_objs PUBLIC src)

include(${CMAKE_CURRENT_SOURCE_DIR}/src/gen/CMakeLists.txt)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs ${PROJECT_NAME}_gen)


add_executable(${PROJECT_NAME}_unittest
    src/tests/test_auth_service.cpp
    src/tests/mock_repositories.hpp
    src/tests/mock_repositories.cpp
    src/tests/test_idm_service.cpp
)
target_link_libraries(${PROJECT_NAME}_unittest PRIVATE
    ${PROJECT_NAME}_objs
    ${PROJECT_NAME}_gen
    userver::utest
)
add_google_tests(${PROJECT_NAME}_unittest)

# Functional Tests
include(UserverTestsuite)

userver_testsuite_add_simple()

# Install
include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
  message(STATUS "Set install prefix: $ENV{PREFIX}")
  file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
  set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
