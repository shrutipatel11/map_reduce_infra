#pragma once

#include <vector>
#include "mapreduce_spec.h"

struct shardInfo {
	std::string f_name;
	int starts_at;
	int ends_at;
};

/* CS6210_TASK: Create your own data structure here, where you can hold information about file splits,
that your master would use for its own bookkeeping and to convey the tasks to the workers for mapping */
struct FileShard {
    int shard_id;
	std::vector<struct shardInfo> info;
};

/* CS6210_TASK: Create fileshards from the list of input files, map_kilobytes etc. using mr_spec you populated  */ 
inline bool shard_files(const MapReduceSpec& mr_spec, std::vector<FileShard>& fileShards) {
	std::vector<std::string> input_files = mr_spec.input_files;
	int shard_size_bytes = 1000 * mr_spec.map_kilobytes;
	int start = 0, bytes_read = 0, id_count = 0, offset = 0;
	std::vector<struct shardInfo> s_info;
	for(int i=0; i<input_files.size(); i++) {
		offset = 0;
		std::ifstream inputFile(input_files[i]);
		if(inputFile.is_open()) {
			std::string line;
			while(std::getline(inputFile, line)) {
				int len = line.length();
				if(bytes_read == 0 || len + bytes_read <= shard_size_bytes ) {
					bytes_read += len;
					if(offset){
						bytes_read += 1;
						offset += 1;
					}
					offset += len;
				} else {
					struct FileShard file_shard;
					file_shard.shard_id = id_count;
					id_count += 1;
					if(offset) {
						struct shardInfo temp;
						temp.f_name = input_files[i];
						temp.starts_at = start;
						temp.ends_at = start + offset - 1;
						s_info.push_back(temp);
						start = start + offset + 1;
					} else {
						start = 0;
					}
					file_shard.info = s_info;
					fileShards.push_back(file_shard);
					bytes_read = len;
					offset = len;
					s_info.clear();
				}
			}

			struct shardInfo temp;
			temp.f_name = input_files[i];
			temp.starts_at = start;
			temp.ends_at = start + offset - 1;
			s_info.push_back(temp);
			offset = 0;
			start = 0;
		}
		inputFile.close();
	}
	if(s_info.size()) {
		struct FileShard file_shard;
		file_shard.shard_id = id_count;
		id_count += 1; 
		file_shard.info = s_info;
		fileShards.push_back(file_shard);
		s_info.clear();
	}

	// int n = fileShards.size();
	// for(int k=0; k<n; k++){
	// 	std::cout<<fileShards[k].shard_id<<std::endl;
	// 	for(int m=0; m<fileShards[k].info.size(); m++){
	// 		std::cout<<fileShards[k].info[m].f_name<<" "<<fileShards[k].info[m].starts_at<<" "<<fileShards[k].info[m].ends_at<<std::endl;
	// 	}
	// }
	return true;
}
