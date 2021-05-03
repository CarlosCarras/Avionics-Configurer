#include "Configurer.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

/* global flags */
bool reg_found = false;


int getLineNo(const std::string& target) {
	std::ifstream configFile(D3_CONFIG_FILE);
	int lineno = 0;

	reg_found = false;		// resetting flag

	if (!configFile) {
		std::cout << "ERROR: D3 avionics configuration file was not found." << std::endl;
		return -1;
	}

	std::string line;
	while(getline(configFile, line)) {
		lineno++;
		size_t found = line.find(target);
		if (found != std::string::npos) {
			/* the device was found */
			reg_found = true;
			break;
		}
	}
	configFile.close();

	if (!reg_found) {
		std::cout << "ERROR: The configuration register for target '" << target << "' was not found." << std::endl;
		return -2;
	}

	return lineno;
}

uint8_t getConfig(const std::string& device, const std::string& reg) {
	std::ifstream configFile(D3_CONFIG_FILE);
	uint8_t val;

	/* getting the line number of the occurence of the target */
	std::string target = device + ":" + reg;
	int lineno = getLineNo(target);
	if (lineno < 0) return CONFIG_REG_ERROR; 						// error

	if (!configFile) {
		std::cout << "ERROR: D3 avionics configuration file was unable to be opened." << std::endl;
		return CONFIG_REG_ERROR;
	}
	
	std::string line;
	for (int i = 0; i < lineno; i++) getline(configFile, line);	// incrementing until target line is reached
	std::size_t found = line.find_last_of(":");
	if (found != std::string::npos) {
		line = line.substr(found+1);							// getting the value as a string
		val = (uint8_t)std::stoi(line, nullptr, 0);				// getting the value as a hex number
	}
	configFile.close();

   return val;
}

int setConfig(const std::string& device, const std::string& reg, uint8_t val) {
 	unsigned int line_cntr = 1;

	/* getting the line number of the occurence of the target */
	std::string target = device + ":" + reg;
	int lineno = getLineNo(target);
	if (lineno < 0) return CONFIG_REG_ERROR; 						// error

	std::ifstream configFile(D3_CONFIG_FILE);
    std::ofstream temp(TEMP_CONFIG_FILE);
    if(!configFile || !temp) {
        std::cout << "ERROR: Unable to open the necessary configuration files." << std::endl;
        return -1;
    }

    std::string line;
	while(getline(configFile, line)) {
		if (line_cntr++ == lineno) {
			char hex_str[2];
			sprintf(hex_str, "%X", val);
			line = target + ":" + hex_str;
		}
		temp << line;
	}

	configFile.close();
	temp.close();

	remove(D3_CONFIG_FILE);
	rename(TEMP_CONFIG_FILE, D3_CONFIG_FILE);

	return 0;
}