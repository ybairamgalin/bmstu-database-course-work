#include <userver/utils/daemon_run.hpp>
#include <aws/core/Aws.h>

#include "components.hpp"

int main(int argc, char* argv[]) {
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    int rc = userver::utils::DaemonMain(argc, argv, MakeComponents());
    Aws::ShutdownAPI(options);
    return rc;
}
