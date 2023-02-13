#pragma once

#include <string>
#include <iostream>

//Added
#include <fstream>
#include <vector>

/* CS6210_TASK Implement this data structureas per your implementation.
		You will need this when your worker is running the map task*/
struct BaseMapperInternal {

		/* DON'T change this function's signature */
		BaseMapperInternal();

		/* DON'T change this function's signature */
		void emit(const std::string& key, const std::string& val);

		/* NOW you can add below, data members and member functions as per the need of your implementation*/
		std::vector<std::string> mapfiles;
};


/* CS6210_TASK Implement this function */
inline BaseMapperInternal::BaseMapperInternal() {
	//Nothing to do
}


/* CS6210_TASK Implement this function */
inline void BaseMapperInternal::emit(const std::string& key, const std::string& val) {
	// std::cout << "Dummy emit by BaseMapperInternal: " << key << ", " << val << std::endl;
	std::string fname = "intermediate/" + key + ".txt";
    std::ofstream output_file;
    output_file.open(fname, std::ios_base::app);
    output_file << val + "\n"; 
    output_file.close();
	mapfiles.push_back(key+".txt");
}


/*-----------------------------------------------------------------------------------------------*/


/* CS6210_TASK Implement this data structureas per your implementation.
		You will need this when your worker is running the reduce task*/
struct BaseReducerInternal {

		/* DON'T change this function's signature */
		BaseReducerInternal();

		/* DON'T change this function's signature */
		void emit(const std::string& key, const std::string& val);

		/* NOW you can add below, data members and member functions as per the need of your implementation*/
		std::string output_dir;
		int num_file; 
};


/* CS6210_TASK Implement this function */
inline BaseReducerInternal::BaseReducerInternal() {
	//Nothing to do
}


/* CS6210_TASK Implement this function */
inline void BaseReducerInternal::emit(const std::string& key, const std::string& val) {
	// std::cout << "Dummy emit by BaseReducerInternal: " << key << ", " << val << std::endl;
	std::string num = std::to_string(num_file);
	std::string fname = output_dir + "/" + num + ".txt";
    std::ofstream output_file;
    output_file.open(fname, std::ios_base::app);
    output_file << key + " " + val + "\n"; 
    output_file.close();
}
