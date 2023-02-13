#pragma once

#include "mapreduce_spec.h"
#include "file_shard.h"
#include <grpc++/grpc++.h>
#include "masterworker.grpc.pb.h"

#include <vector>
#include <unistd.h>
#include <sys/stat.h>
#include <experimental/filesystem>
#include <chrono>
namespace fs = std::experimental::filesystem;

using masterworker::MasterMachine;
using masterworker::FShard;
using masterworker::ShardInfo;
using masterworker::MapRequest;
using masterworker::ReduceRequest;
using masterworker::FilePaths;
using masterworker::MasterRequest;
using masterworker::WorkerResponse;

using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Channel;
using grpc::Status;

/* CS6210_TASK: Handle all the bookkeeping that Master is supposed to do.
	This is probably the biggest task for this project, will test your understanding of map reduce */
class Master {

	public:
		/* DON'T change the function signature of this constructor */
		Master(const MapReduceSpec&, const std::vector<FileShard>&);

		/* DON'T change this function's signature */
		bool run();

	private:
		/* NOW you can add below, data members and member functions as per the need of your implementation*/
		MapReduceSpec mapRedSpec;
		std::vector<FileShard> fileShards;
		std::vector<std::string> temp_file_paths;
		int current_shard_id, num_occupied_workers, current_output_id;
		std::map<std::string, int> mapfiles;
		int failed_worker_id = -1;
		int failed_worker_id_2 = -1;
		std::chrono::system_clock::time_point deadline;

		struct RPC_Call {
			WorkerResponse response;
			ClientContext context;
			Status status;
			int worker_id;
			std::unique_ptr<ClientAsyncResponseReader<WorkerResponse>> async_resp_reader;
    	};

    	std::vector<std::unique_ptr<MasterMachine::Stub>>stubs_;
    	CompletionQueue cq;

		void executeMapPhase();
		void executeMapAgain(); //In case of failures
		void executeReducePhase();
		void executeReduceAgain();  //In case of failures
};

/* CS6210_TASK: This is all the information your master will get from the framework.
You can populate your other class data members here if you want */
Master::Master(const MapReduceSpec& mr_spec, const std::vector<FileShard>& file_shards) {
	mapRedSpec = mr_spec;
	fileShards = file_shards;
	current_shard_id = 0;
	num_occupied_workers = 0;
	current_output_id = 0;
	fs::remove_all("intermediate");
	sleep(17);
	auto result = fs::create_directory("intermediate");
	if(!result) std::cout<<"Error creating intermediate directory"<<std::endl;

	fs::remove_all(mr_spec.output_dir);
	auto result2 = fs::create_directory(mr_spec.output_dir);
	if(!result2) std::cout<<"Error creating output directory"<<std::endl;

	for(auto ip_addr_port : mr_spec.worker_ipaddr_ports){
        stubs_.emplace_back(MasterMachine::NewStub(grpc::CreateChannel(ip_addr_port, grpc::InsecureChannelCredentials()))); 
    }
}

void Master::executeMapPhase(){
	std::vector<int> visited;
	int num_shards = fileShards.size();
	int worker_id = 0;
	// std::cout<<"length : "<<mapRedSpec.worker_ipaddr_ports.size()<<std::endl;
	for(auto worker: mapRedSpec.worker_ipaddr_ports) {
        if(current_shard_id >= num_shards) break;
		MasterRequest request;
		request.set_user_id(mapRedSpec.user_id);
		request.set_phase("map");
		MapRequest *map_request = request.mutable_map_request();
		FShard *f_shard = map_request->mutable_shard();
		int sid = fileShards[current_shard_id].shard_id;
		// std::cout<<"Shard ID :"<<std::to_string(sid)<<std::endl;
        f_shard->set_shard_id(sid);
		
		int num_info = fileShards[current_shard_id].info.size();
        for(int i=0; i<num_info; i++) {
        	f_shard->add_info();
        	ShardInfo *info = f_shard->mutable_info(i);
			shardInfo sinfo = fileShards[current_shard_id].info[i];
        	info->set_starts_at(sinfo.starts_at);
			info->set_ends_at(sinfo.ends_at);
			info->set_f_name(sinfo.f_name);
        }

		visited.push_back(current_shard_id);
		RPC_Call* call = new RPC_Call;
		call->worker_id = worker_id;
		// std::cout<<"Mapping : "<<worker<<" - "<<worker_id<<std::endl;
		deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(10000);
		call->context.set_deadline(deadline);
  		call->async_resp_reader = stubs_[worker_id]->PrepareAsyncmapReduceRequest(&call->context, request, &cq);
		call->async_resp_reader->StartCall();
  		call->async_resp_reader->Finish(&call->response, &call->status, (void*)call);
		num_occupied_workers += 1;
		current_shard_id += 1;
		worker_id += 1;
	}

	// std::cout<<"Reached here"<<std::endl;
	void* got_tag;
  	bool ok = false;
	WorkerResponse response;
	Status status = Status::OK;

	while (cq.Next(&got_tag, &ok)) {
		RPC_Call* call = static_cast<RPC_Call*>(got_tag);
		num_occupied_workers -= 1;
		GPR_ASSERT(ok);

		// std::cout<<"Reached here 1"<<std::endl;
		if (call->status.ok())  {
			// std::cout<<"Reached here 2"<<std::endl;
			for (const auto result : call->response.paths().f_name()) {
				//some files missing in mapfiles
		    	mapfiles.insert({result, 1});
			}
			// std::cout<<"NUm occupied workers : "<<num_occupied_workers<<" : "<<current_shard_id<<" : "<<num_shards<<std::endl;
			if (current_shard_id >= num_shards && num_occupied_workers==0) break;
			
	        if(current_shard_id < num_shards) {
				MasterRequest request;
				request.set_phase("map");
				request.set_user_id(mapRedSpec.user_id);
				MapRequest *map_request = request.mutable_map_request();
				FShard *f_shard = map_request->mutable_shard();
				int sid = fileShards[current_shard_id].shard_id;
				// std::cout<<"Shard ID 2:"<<std::to_string(sid)<<std::endl;
        		f_shard->set_shard_id(sid);

				int num_info = fileShards[current_shard_id].info.size();
        		for(int i=0; i<num_info; i++) {
        			f_shard->add_info();
        			ShardInfo *info = f_shard->mutable_info(i);
        			shardInfo sinfo = fileShards[current_shard_id].info[i];
        			info->set_starts_at(sinfo.starts_at);
					info->set_ends_at(sinfo.ends_at);
					info->set_f_name(sinfo.f_name);
					// std::cout<<sinfo.f_name<<" "<<sinfo.starts_at<<" "<<sinfo.ends_at<<std::endl;
        		}

				RPC_Call* c = new RPC_Call;
				// std::cout<<"Worker ID : "<<std::to_string(call->worker_id)<<" "<<std::to_string(stubs_.size())<<std::endl;
				c->worker_id = call->worker_id;
				deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(10000);
				c->context.set_deadline(deadline);
  				c->async_resp_reader = stubs_[call->worker_id]->PrepareAsyncmapReduceRequest(&c->context, request, &cq);
				c->async_resp_reader->StartCall();
  				c->async_resp_reader->Finish(&c->response, &c->status, (void*)c);
				num_occupied_workers += 1;
				current_shard_id += 1;
			}
		} else{
			//Failed worker
			failed_worker_id = call->worker_id;
			// std::cout<<"Failed worker "<<failed_worker_id<<std::endl;
			std::cout << status.error_code() << ": " << status.error_message()<< std::endl;
			status = call->status;
		} 
		delete call;
	}

	// std::cout<<"Outside while loop"<<std::endl;
	std::cout<<failed_worker_id<<std::endl;
	if(failed_worker_id != -1) {
		std::vector<std::string>::iterator it = mapRedSpec.worker_ipaddr_ports.begin() + failed_worker_id;
		mapRedSpec.worker_ipaddr_ports.erase(it);
		failed_worker_id = -1;
		executeMapAgain();
	}
    
}

void Master::executeMapAgain(){
	// std::cout<<"Executing map again for failure"<<std::endl;
	current_shard_id = 0;
	num_occupied_workers = 0;
	current_output_id = 0;
	mapfiles.clear();
	fs::remove_all("intermediate");
	// system("/bin/rm -rf imtermediate");
	sleep(17);
	auto result = fs::create_directory("intermediate");
	if(!result) std::cout<<"Error creating intermediate directory"<<std::endl;

	fs::remove_all(mapRedSpec.output_dir);
	auto result2 = fs::create_directory(mapRedSpec.output_dir);
	if(!result2) std::cout<<"Error creating output directory"<<std::endl;

	stubs_.clear();
	for(auto ip_addr_port : mapRedSpec.worker_ipaddr_ports){
        stubs_.emplace_back(MasterMachine::NewStub(grpc::CreateChannel(ip_addr_port, grpc::InsecureChannelCredentials()))); 
    }

	// std::cout<<"Getting to map"<<std::endl;
	executeMapPhase();
}

void Master::executeReducePhase(){
	int worker_id = 0;
	for(auto it=mapfiles.begin(); it!=mapfiles.end(); it++){
			temp_file_paths.push_back(it->first);
	}
	int split_size = (temp_file_paths.size()/mapRedSpec.n_output_files) + 1;
	for(auto worker: mapRedSpec.worker_ipaddr_ports) {
        if(current_output_id >= split_size) break;
        MasterRequest request;
		request.set_user_id(mapRedSpec.user_id);
		request.set_phase("reduce");
		ReduceRequest *reduce_request = request.mutable_reduce_request();
		reduce_request->set_out_dir(mapRedSpec.output_dir);
		reduce_request->set_id(current_output_id);
		FilePaths *fpaths = reduce_request->mutable_paths();

		int l = temp_file_paths.size();
        for(int i=0 ; i<split_size && current_output_id * split_size + i < l; i++){
			std::string addingFile = temp_file_paths[current_output_id * split_size + i];
			// std::cout<<"File to be added : "<<addingFile<<std::endl;
        	fpaths->add_f_name(addingFile);
        }

		RPC_Call* rcall = new RPC_Call;
		// std::cout<<"First worker Id : "<<worker_id<<std::endl;
		rcall->worker_id = worker_id;
		deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(10000);
		rcall->context.set_deadline(deadline);
  		rcall->async_resp_reader = stubs_[worker_id]->PrepareAsyncmapReduceRequest(&rcall->context, request, &cq);
		rcall->async_resp_reader->StartCall();
  		rcall->async_resp_reader->Finish(&rcall->response, &rcall->status, (void*)rcall);
		current_output_id += 1;
		worker_id += 1;
	}

	void* got_tag;
  	bool ok = false;
	WorkerResponse response;
	Status status = Status::OK;

	while (cq.Next(&got_tag, &ok)) {
		RPC_Call* icall = static_cast<RPC_Call*>(got_tag);

		GPR_ASSERT(ok);
		if (icall->status.ok())  {
			if(current_output_id >= mapRedSpec.n_output_files)
				break;

			MasterRequest req;
			req.set_user_id(mapRedSpec.user_id);
			req.set_phase("reduce");
			ReduceRequest *reduce_req = req.mutable_reduce_request();
			reduce_req->set_out_dir(mapRedSpec.output_dir);
			reduce_req->set_id(current_output_id);
			FilePaths *fp = reduce_req->mutable_paths();

			int l = temp_file_paths.size();
			for(int i=0 ; i<split_size && current_output_id * split_size + i < l; i++){
				// std::cout<<split_size<<" "<<current_output_id<<" "<<split_size<<" "<<i<<" "<<l<<std::endl;
				std::string addingFile = temp_file_paths[current_output_id * split_size + i];
				// std::cout<<"File to be added 2: "<<addingFile<<std::endl;
				fp->add_f_name(addingFile);
			}

			RPC_Call* rc = new RPC_Call;
			// std::cout<<stubs_.size()<<" "<<icall->worker_id<<" "<<icall->status.ok()<<std::endl;
			rc->worker_id = icall->worker_id;
			deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(10000);
			rc->context.set_deadline(deadline);
			rc->async_resp_reader = stubs_[icall->worker_id]->PrepareAsyncmapReduceRequest(&rc->context, req, &cq);
			rc->async_resp_reader->StartCall();
			rc->async_resp_reader->Finish(&rc->response, &rc->status, (void*)rc);
			current_output_id++;
		} else {
			int failed_worker_id_2 = icall->worker_id;
			executeReduceAgain();
			status = icall->status;
		}
		delete icall;
	}

	// std::cout<<"Outside while loop"<<std::endl;
	// std::cout<<failed_worker_id_2<<std::endl;
	if(failed_worker_id_2 != -1) {
		temp_file_paths.clear();
		std::vector<std::string>::iterator it = mapRedSpec.worker_ipaddr_ports.begin() + failed_worker_id_2;
		mapRedSpec.worker_ipaddr_ports.erase(it);
		failed_worker_id_2 = -1;
		executeReduceAgain();
	}
}

void Master::executeReduceAgain(){
	// std::cout<<"Executing reduce again for failure"<<std::endl;
	fs::remove_all(mapRedSpec.output_dir);
	auto result2 = fs::create_directory(mapRedSpec.output_dir);
	if(!result2) std::cout<<"Error creating output directory"<<std::endl;

	stubs_.clear();
	for(auto ip_addr_port : mapRedSpec.worker_ipaddr_ports){
        stubs_.emplace_back(MasterMachine::NewStub(grpc::CreateChannel(ip_addr_port, grpc::InsecureChannelCredentials()))); 
    }

	executeReducePhase();
}

/* CS6210_TASK: Here you go. once this function is called you will complete whole map reduce task and return true if succeeded */
bool Master::run() {
	// std::cout<<"Map Phase : Started"<<std::endl;
	executeMapPhase();
	sleep(5);
	// std::cout<<"Map Phase : Ended"<<std::endl;
	// std::cout<<"Reduce Phase : Started"<<std::endl;
	executeReducePhase();
	sleep(2);
	// std::cout<<"Reduce Phase : Ended"<<std::endl;
	return true;
}