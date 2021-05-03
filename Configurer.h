
#ifndef CONFIGURER_H
#define CONFIGURER_H

#include <cstdint>
#include <string>

#define D3_CONFIG_FILE   "config.d3"
#define TEMP_CONFIG_FILE "temp.d3"

#define CONFIG_REG_ERROR 0xFF

int getLineNo(const std::string& target);
uint8_t getConfig(const std::string& device, const std::string& reg);
int setConfig(const std::string& device, const std::string& reg, uint8_t val);

#endif // CONFIGURER_H