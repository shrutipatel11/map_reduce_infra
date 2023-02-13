#pragma once

#include <mr_task_factory.h>
#include "mr_tasks.h"
#include <string>
#include <fstream>

#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>
#include <grpc++/grpc++.h>
#include "masterworker.grpc.pb.h"
#include "masterworker.pb.h"

using masterworker::MasterMachine;
using masterworker::FShard;
using masterworker::ShardInfo;
using masterworker::MapRequest;
using masterworker::ReduceRequest;
using masterworker::FilePaths;
using masterworker::MasterRequest;
using masterworker::WorkerResponse;

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;

extern std::shared_ptr<BaseMapper> get_mapper_from_task_factory(const std::string& user_id);
extern std::shared_ptr<BaseReducer> get_reducer_from_task_factory(const std::string& user_id);

/* CS6210_TASK: Handle all the task a Worker is supposed to do.
	This is a big task for this project, will test your understanding of map reduce */
class Worker {

	public:
		/* DON'T change the function signature of this constructor */
		Worker(std::string ip_addr_port);

		/* DON'T change this function's signature */
		bool run();

	private:
		/* NOW you can add below, data members and member functions as per the need of your implementation*/
		std::string worker_ip_and_port;

		class MapReduceService final {
			public:
				~MapReduceService() {
		    		server_->Shutdown();
		    		cq_->Shutdown();
		  		}

				void Run(std::string ip_and_port) {
					ServerBuilder builder;
					std::string worker_ip_and_port(ip_and_port);
					builder.AddListeningPort(worker_ip_and_port, grpc::InsecureServerCredentials());
					builder.RegisterService(&service_);
					cq_ = builder.AddCompletionQueue();
					server_ = builder.BuildAndStart();

					new CallData(&service_, cq_.get());
					bool ok;
					void* tag;  
					while(1) {
						GPR_ASSERT(cq_->Next(&tag, &ok));
						GPR_ASSERT(ok);
						static_cast<CallData*>(tag)->Proceed();
					}
				}

			private:
		  		class CallData {
		   			public:
		    			CallData(MasterMachine::AsyncService* service, ServerCompletionQueue* cq)
		        			: service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
		      				Proceed();
		    			}
		        
		        		void Proceed() {
		      				if (status_ == CREATE) {
		        				status_ = PROCESS;
		        				service_->RequestmapReduceRequest(&ctx_, &request_, &responder_, cq_, cq_, this);
		      				} 
							else if (status_ == PROCESS) {
			      				new CallData(service_, cq_);
								if(request_.phase() == "map") {
									executeMapPhase();
		        				} 
								else if (request_.phase() == "reduce") {
		        					executeReducePhase();
		        				}
		      				} 
							else {
		        				GPR_ASSERT(status_ == FINISH);
		        				delete this;
		     				}
		    			}

						void executeMapPhase(){
							// std::cout<<"Executing map phase "<<std::endl;
							auto mapper = get_mapper_from_task_factory(request_.user_id());
			        		FShard sh = request_.map_request().shard();
							for(int i=0; i<sh.info().size(); i++) {
								ShardInfo shInfo = sh.info()[i];
								std::string fname = shInfo.f_name();
		        				std::ifstream inputFile(fname);
								if(inputFile.is_open()) {
				        			int start_pos = shInfo.starts_at();
				        			int end_pos = shInfo.ends_at();
									// std::cout<<start_pos<<" "<<end_pos<<std::endl;
									char line[end_pos - start_pos + 2];
									inputFile.seekg(start_pos, std::ios::beg);
									inputFile.read(line, end_pos - start_pos + 1);
									line[end_pos - start_pos + 1] = 0;
									std::string final_line = line;
									std::size_t index = 0;
									// std::string eof = std::string::npos;
    								while ((index = final_line.find("\n", index)) != std::string::npos) {
         								final_line.replace(index, 1, " ");
         								index += 1;
    								}

									//Debug code
									// std::string debugfile = fname+std::to_string(start_pos)+".txt";
    								// std::ofstream debug_out;
    								// debug_out.open(debugfile, std::ios_base::app);
    								// debug_out << final_line + "\n"; 
    								// debug_out.close();
									//Debug over

									mapper->map(final_line);
									inputFile.close();
								}		
							}
			        		std::vector<std::string> files = mapper->impl_->mapfiles;
							FilePaths *paths = reply_.mutable_paths();

							for(int i=0;i<files.size();i++){
								paths->add_f_name(files[i]);
								// cout<<"Adding file paths : "<<fileNames[i]<<endl;
							}
							status_ = FINISH;
							responder_.Finish(reply_, Status::OK, this);
						}

						void executeReducePhase(){
							auto reducer = get_reducer_from_task_factory(request_.user_id());
		        			reducer->impl_->num_file = request_.reduce_request().id();
		        			reducer->impl_->output_dir = request_.reduce_request().out_dir();
							int count = 0;
		        			for(const auto mapfile : request_.reduce_request().paths().f_name()){
								// std::cout<<"Reduce filenames : "<<mapfile<<std::endl;
								int len = mapfile.length();
								std::vector<std::string> values;
								std::vector<std::string> final_value;
		        				std::ifstream inputFile("intermediate/" + mapfile);
								if(inputFile.is_open()) {
									std::string line;
									while(getline(inputFile, line)) {
		        						values.push_back(line);
		        					}
								}
								int val = values.size();
								final_value.push_back(std::to_string(val));
								std::string key = mapfile.substr(0,len-4);
								// cout<<"key "<<key<<endl;
		        				reducer->reduce(key, values);
								inputFile.close();
		        			}
		        			FilePaths *paths = reply_.mutable_paths();
							std::string addFile = std::to_string(request_.reduce_request().id());
							paths->add_f_name(addFile + ".txt");
		        			status_ = FINISH;
							responder_.Finish(reply_, Status::OK, this);
						}

		   			private:
		    			MasterMachine::AsyncService* service_;
		    			ServerCompletionQueue* cq_;
		    			ServerContext ctx_;
		    			MasterRequest request_;
		    			WorkerResponse reply_;
		    			ServerAsyncResponseWriter<WorkerResponse> responder_;
		    			enum CallStatus { CREATE, PROCESS, FINISH };
		    			CallStatus status_;
		  		};

		  		std::unique_ptr<ServerCompletionQueue> cq_;
		  		MasterMachine::AsyncService service_;
		  		std::unique_ptr<Server> server_;
		};
};


/* CS6210_TASK: ip_addr_port is the only information you get when started.
	You can populate your other class data members here if you want */
Worker::Worker(std::string ip_addr_port) {
	worker_ip_and_port = ip_addr_port;
}

/* CS6210_TASK: Here you go. once this function is called your woker's job is to keep looking for new tasks 
	from Master, complete when given one and again keep looking for the next one.
	Note that you have the access to BaseMapper's member BaseMapperInternal impl_ and 
	BaseReduer's member BaseReducerInternal impl_ directly, 
	so you can manipulate them however you want when running map/reduce tasks*/
bool Worker::run() {
	/*  Below 5 lines are just examples of how you will call map and reduce
		Remove them once you start writing your own logic */ 
	// std::cout << "worker.run(), I 'm not ready yet" <<std::endl;
	// auto mapper = get_mapper_from_task_factory("cs6210");
	// mapper->map("I m just a 'dummy', a \"dummy line\"");
	// auto reducer = get_reducer_from_task_factory("cs6210");
	// reducer->reduce("dummy", std::vector<std::string>({"1", "1"}));
	// return true;

	MapReduceService service;
	service.Run(worker_ip_and_port);
	return true;
}
