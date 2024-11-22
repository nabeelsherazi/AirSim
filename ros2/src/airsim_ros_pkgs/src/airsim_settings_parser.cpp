#include "airsim_settings_parser.h"
#include <iostream>

AirSimSettingsParser::AirSimSettingsParser(const std::string& host_ip)
    : host_ip_(host_ip)
{
    success_ = initializeSettings();
}

bool AirSimSettingsParser::success()
{
    return success_;
}

bool AirSimSettingsParser::getSettingsText(std::string& settings_text) const
{
    msr::airlib::RpcLibClientBase airsim_client(host_ip_);
    airsim_client.confirmConnection();

    settings_text = airsim_client.getSettingsString();

    std::cerr << "settings text" << settings_text << std::endl;

    return !settings_text.empty();
}

std::string AirSimSettingsParser::getSimMode()
{
    const auto& settings_json = msr::airlib::Settings::loadJSonString(settings_text_);
    return settings_json.getString("SimMode", "Multirotor");
}

// mimics void ASimHUD::initializeSettings()
bool AirSimSettingsParser::initializeSettings()
{
    if (getSettingsText(settings_text_)) {
        AirSimSettings::initializeSettings(settings_text_);

        AirSimSettings::singleton().load(std::bind(&AirSimSettingsParser::getSimMode, this));
        std::cout << "SimMode: " << AirSimSettings::singleton().simmode_name << std::endl;

        return true;
    }

    return false;
}
