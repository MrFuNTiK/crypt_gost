#include <ini/ini_config.hpp>
#include <ini/ini_parser.hpp>
#include <kat_environment.hpp>
#include <filesystem>
#include <fstream>
#include <stdexcept>

using namespace crypt_gost::test::kat;

TestEnvironment& TestEnvironment::Instance()
{
    static TestEnvironment env;
    return env;
}

void TestEnvironment::RegisterConfig( test_algo alg, IniConfig&& config )
{
    configs_[ ToString(alg) ] = config;
}

void TestEnvironment::LoadConfigs(const std::string& data_path)
{
    std::filesystem::path data_dir_path(data_path);
    if(data_dir_path.has_filename()) {
        throw std::logic_error("must be a path to data directory");
    }
    std::ifstream ifs(data_path + "data.ini");
    auto dataIni = ParseIni(ifs);
    const auto& test_configs = dataIni.Section("data_ini_locations");

    for(test_algo alg = BEGIN; alg < COUNT; ++alg) {
        auto file_path = data_path + test_configs.Property(ToString(alg));
        std::ifstream test_ini_file(file_path);
        if(!test_ini_file.is_open()) {
            throw std::logic_error("file not exists");
        }
        configs_[ToString(alg)] = ParseIni(test_ini_file);
    }
}

const IniConfig& TestEnvironment::GetConfig( test_algo alg )
{
    return configs_[ ToString(alg) ];
}
