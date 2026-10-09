#include <gtest/gtest.h>
#include "program_options.hpp"
#include "kat_environment.hpp"

static const std::vector<program_arg> ARGS = {
    {
        .name = "config-ini",
        .description = "config with links to test data",
        .hasValue = true,
    },
};

int main( int argc, const char** argv )
{
    auto& env = crypt_gost::test::kat::TestEnvironment::Instance();
    OptionsParser options_parser(ARGS);
    auto options = options_parser.ParseArgs(argc, argv);
    env.LoadConfigs(options[ARGS[0].name]);

    testing::InitGoogleTest( &argc, const_cast<char**>(argv) );
    return RUN_ALL_TESTS();
}
