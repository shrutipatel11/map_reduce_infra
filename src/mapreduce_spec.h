#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>


/* CS6210_TASK: Create your data structure here for storing spec from the config file */
struct MapReduceSpec {
	int n_workers;
	std::vector<std::string> worker_ipaddr_ports;
	std::vector<std::string> input_files;
	std::string output_dir;
	int n_output_files;
	int map_kilobytes;
	std::string user_id;
};

/* CS6210_TASK: Populate MapReduceSpec data structure with the specification from the config file */
inline bool read_mr_spec_from_config_file(const std::string& config_filename, MapReduceSpec& mr_spec) {
	std::ifstream configFile(config_filename);
	// FILE* fp = fopen(config_filename, "r");
	// if(fp == NULL) exit(EXIT_FAILURE);
	if(configFile.is_open()) {
		std::string line;
		while(std::getline(configFile, line)) {
			int splitIndex = line.find("=");
			std::string attribute = line.substr(0, splitIndex);
			std::string value = line.substr(splitIndex+1);
			if(attribute == "n_workers") mr_spec.n_workers = stoi(value);
			else if(attribute == "output_dir") mr_spec.output_dir = value;
			else if(attribute == "n_output_files") mr_spec.n_output_files = stoi(value);
			else if(attribute == "map_kilobytes") mr_spec.map_kilobytes = stoi(value);
			else if(attribute == "user_id") mr_spec.user_id = value;
			else if(attribute == "worker_ipaddr_ports") {
				std::string tempStr = "";
				int l = value.length();
				for(int i=0; i<l; i++){
					if(value[i] == ','){
						mr_spec.worker_ipaddr_ports.push_back(tempStr);
						tempStr = "";
					} else {
						tempStr = tempStr + value[i];
					}
				}
				mr_spec.worker_ipaddr_ports.push_back(tempStr);
			}
			else if(attribute == "input_files") {
				std::string tempStr = "";
				int l = value.length();
				for(int i=0; i<l; i++){
					if(value[i] == ','){
						mr_spec.input_files.push_back(tempStr);
						tempStr = "";
					} else {
						tempStr = tempStr + value[i];
					}
				}
				mr_spec.input_files.push_back(tempStr);
				// for(int i=0; i<mr_spec.input_files.size(); i++){
				// 	std::cout<<"Input files "<<mr_spec.input_files[i]<<std::endl;
				// }
			}
		}
		configFile.close();
	}
	return true;
}


/* CS6210_TASK: validate the specification read from the config file */
inline bool validate_mr_spec(const MapReduceSpec& mr_spec) {
	if(mr_spec.n_workers == 0) return false;
	else if (mr_spec.output_dir == "") return false;
	else if (mr_spec.n_output_files == 0) return false;
	else if (mr_spec.map_kilobytes == 0) return false;
	else if (mr_spec.user_id == "") return false;
	else if (mr_spec.worker_ipaddr_ports.size() == 0) return false;
	else if (mr_spec.input_files.size() == 0) return false;
	else return true;
}
