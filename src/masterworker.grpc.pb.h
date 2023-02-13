// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: masterworker.proto
#ifndef GRPC_masterworker_2eproto__INCLUDED
#define GRPC_masterworker_2eproto__INCLUDED

#include "masterworker.pb.h"

#include <functional>
#include <grpc/impl/codegen/port_platform.h>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace masterworker {

class MasterMachine final {
 public:
  static constexpr char const* service_full_name() {
    return "masterworker.MasterMachine";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status mapReduceRequest(::grpc::ClientContext* context, const ::masterworker::MasterRequest& request, ::masterworker::WorkerResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::masterworker::WorkerResponse>> AsyncmapReduceRequest(::grpc::ClientContext* context, const ::masterworker::MasterRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::masterworker::WorkerResponse>>(AsyncmapReduceRequestRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::masterworker::WorkerResponse>> PrepareAsyncmapReduceRequest(::grpc::ClientContext* context, const ::masterworker::MasterRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::masterworker::WorkerResponse>>(PrepareAsyncmapReduceRequestRaw(context, request, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      virtual void mapReduceRequest(::grpc::ClientContext* context, const ::masterworker::MasterRequest* request, ::masterworker::WorkerResponse* response, std::function<void(::grpc::Status)>) = 0;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void mapReduceRequest(::grpc::ClientContext* context, const ::masterworker::MasterRequest* request, ::masterworker::WorkerResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void mapReduceRequest(::grpc::ClientContext* context, const ::masterworker::MasterRequest* request, ::masterworker::WorkerResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
    };
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    typedef class experimental_async_interface async_interface;
    #endif
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    async_interface* async() { return experimental_async(); }
    #endif
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::masterworker::WorkerResponse>* AsyncmapReduceRequestRaw(::grpc::ClientContext* context, const ::masterworker::MasterRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::masterworker::WorkerResponse>* PrepareAsyncmapReduceRequestRaw(::grpc::ClientContext* context, const ::masterworker::MasterRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status mapReduceRequest(::grpc::ClientContext* context, const ::masterworker::MasterRequest& request, ::masterworker::WorkerResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::masterworker::WorkerResponse>> AsyncmapReduceRequest(::grpc::ClientContext* context, const ::masterworker::MasterRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::masterworker::WorkerResponse>>(AsyncmapReduceRequestRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::masterworker::WorkerResponse>> PrepareAsyncmapReduceRequest(::grpc::ClientContext* context, const ::masterworker::MasterRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::masterworker::WorkerResponse>>(PrepareAsyncmapReduceRequestRaw(context, request, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      void mapReduceRequest(::grpc::ClientContext* context, const ::masterworker::MasterRequest* request, ::masterworker::WorkerResponse* response, std::function<void(::grpc::Status)>) override;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void mapReduceRequest(::grpc::ClientContext* context, const ::masterworker::MasterRequest* request, ::masterworker::WorkerResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void mapReduceRequest(::grpc::ClientContext* context, const ::masterworker::MasterRequest* request, ::masterworker::WorkerResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::masterworker::WorkerResponse>* AsyncmapReduceRequestRaw(::grpc::ClientContext* context, const ::masterworker::MasterRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::masterworker::WorkerResponse>* PrepareAsyncmapReduceRequestRaw(::grpc::ClientContext* context, const ::masterworker::MasterRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_mapReduceRequest_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status mapReduceRequest(::grpc::ServerContext* context, const ::masterworker::MasterRequest* request, ::masterworker::WorkerResponse* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_mapReduceRequest : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_mapReduceRequest() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_mapReduceRequest() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status mapReduceRequest(::grpc::ServerContext* /*context*/, const ::masterworker::MasterRequest* /*request*/, ::masterworker::WorkerResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestmapReduceRequest(::grpc::ServerContext* context, ::masterworker::MasterRequest* request, ::grpc::ServerAsyncResponseWriter< ::masterworker::WorkerResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_mapReduceRequest<Service > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_mapReduceRequest : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_mapReduceRequest() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::masterworker::MasterRequest, ::masterworker::WorkerResponse>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::masterworker::MasterRequest* request, ::masterworker::WorkerResponse* response) { return this->mapReduceRequest(context, request, response); }));}
    void SetMessageAllocatorFor_mapReduceRequest(
        ::grpc::experimental::MessageAllocator< ::masterworker::MasterRequest, ::masterworker::WorkerResponse>* allocator) {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
    #else
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::experimental().GetHandler(0);
    #endif
      static_cast<::grpc::internal::CallbackUnaryHandler< ::masterworker::MasterRequest, ::masterworker::WorkerResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_mapReduceRequest() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status mapReduceRequest(::grpc::ServerContext* /*context*/, const ::masterworker::MasterRequest* /*request*/, ::masterworker::WorkerResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* mapReduceRequest(
      ::grpc::CallbackServerContext* /*context*/, const ::masterworker::MasterRequest* /*request*/, ::masterworker::WorkerResponse* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* mapReduceRequest(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::masterworker::MasterRequest* /*request*/, ::masterworker::WorkerResponse* /*response*/)
    #endif
      { return nullptr; }
  };
  #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
  typedef ExperimentalWithCallbackMethod_mapReduceRequest<Service > CallbackService;
  #endif

  typedef ExperimentalWithCallbackMethod_mapReduceRequest<Service > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_mapReduceRequest : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_mapReduceRequest() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_mapReduceRequest() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status mapReduceRequest(::grpc::ServerContext* /*context*/, const ::masterworker::MasterRequest* /*request*/, ::masterworker::WorkerResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_mapReduceRequest : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_mapReduceRequest() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_mapReduceRequest() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status mapReduceRequest(::grpc::ServerContext* /*context*/, const ::masterworker::MasterRequest* /*request*/, ::masterworker::WorkerResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestmapReduceRequest(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_mapReduceRequest : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_mapReduceRequest() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->mapReduceRequest(context, request, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_mapReduceRequest() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status mapReduceRequest(::grpc::ServerContext* /*context*/, const ::masterworker::MasterRequest* /*request*/, ::masterworker::WorkerResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* mapReduceRequest(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* mapReduceRequest(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_mapReduceRequest : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_mapReduceRequest() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::masterworker::MasterRequest, ::masterworker::WorkerResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::masterworker::MasterRequest, ::masterworker::WorkerResponse>* streamer) {
                       return this->StreamedmapReduceRequest(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_mapReduceRequest() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status mapReduceRequest(::grpc::ServerContext* /*context*/, const ::masterworker::MasterRequest* /*request*/, ::masterworker::WorkerResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedmapReduceRequest(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::masterworker::MasterRequest,::masterworker::WorkerResponse>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_mapReduceRequest<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_mapReduceRequest<Service > StreamedService;
};

}  // namespace masterworker


#endif  // GRPC_masterworker_2eproto__INCLUDED
