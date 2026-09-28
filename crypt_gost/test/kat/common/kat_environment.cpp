#include <ini/ini_config.hpp>
#include <ini/ini_parser.hpp>
#include <kat_environment.hpp>
#include <filesystem>
#include <fstream>
#include <stdexcept>

KAT_environment& KAT_environment::Instance()
{
    static KAT_environment env;
    return env;
}

void KAT_environment::RegisterConfig( test_algo alg, IniConfig&& config )
{
    configs_[ alg ] = config;
}

void KAT_environment::LoadConfigs(const std::string& data_path)
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
        configs_[alg] = ParseIni(test_ini_file);
    }
}

const IniConfig& KAT_environment::GetConfig( test_algo alg )
{
    return configs_[ alg ];
}
