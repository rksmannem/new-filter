// NOLINT(namespace-envoy)
#include <string>
#include <unordered_map>

#include "google/protobuf/util/json_util.h"
#include "proxy_wasm_intrinsics.h"
#include "filter.pb.h"

class AddHeaderRootContext : public RootContext {
public:
  explicit AddHeaderRootContext(uint32_t id, StringView root_id) : RootContext(id, root_id) {}
  bool onConfigure(size_t /* configuration_size */) override;

  bool onStart(size_t) override;

  std::string header_name_;
  std::string header_value_;
};

class AddHeaderContext : public Context {
public:
  explicit AddHeaderContext(uint32_t id, RootContext* root) : Context(id, root), root_(static_cast<AddHeaderRootContext*>(static_cast<void*>(root))) {}

  void onCreate() override;
  FilterHeadersStatus onRequestHeaders(uint32_t headers) override;
  FilterDataStatus onRequestBody(size_t body_buffer_length, bool end_of_stream) override;
  FilterHeadersStatus onResponseHeaders(uint32_t headers) override;
  void onDone() override;
  void onLog() override;
  void onDelete() override;
private:

  AddHeaderRootContext* root_;
};
static RegisterContextFactory register_AddHeaderContext(CONTEXT_FACTORY(AddHeaderContext),
                                                      ROOT_FACTORY(AddHeaderRootContext),
                                                      "add_header_root_id");

bool AddHeaderRootContext::onConfigure(size_t) { 
  auto conf = getConfiguration();
  Config config;
  
  google::protobuf::util::JsonParseOptions options;
  options.case_insensitive_enum_parsing = true;
  options.ignore_unknown_fields = false;

  google::protobuf::util::JsonStringToMessage(conf->toString(), &config, options);
  LOG_INFO("onConfigure name " + config.name());
  LOG_INFO("onConfigure " + config.value());
  header_name_ = config.name();
  header_value_ = config.value();
  return true; 
}

bool AddHeaderRootContext::onStart(size_t) { LOG_INFO("onStart"); return true;}

void AddHeaderContext::onCreate() { LOG_INFO(std::string("onCreate " + std::to_string(id()))); }

FilterHeadersStatus AddHeaderContext::onRequestHeaders(uint32_t) {
  LOG_INFO(std::string("BEGIN:onRequestHeaders ") + std::to_string(id()));

  auto result = getRequestHeaderPairs();
  auto pairs = result->pairs();
  LOG_INFO(std::string("request-headers: ") + std::to_string(pairs.size()));
  for (auto &p : pairs) {
    LOG_INFO(std::string(p.first) + std::string(" -> ") + std::string(p.second));
  }

  LOG_DEBUG(std::string("END:onRequestHeaders ") + std::to_string(id()));
  return FilterHeadersStatus::Continue;
}

FilterDataStatus AddHeaderContext::onRequestBody(size_t body_buffer_length, bool end_of_stream) {
  auto body = getBufferBytes(BufferType::HttpRequestBody, 0, body_buffer_length);
  LOG_INFO(std::string("onRequestBody ") + std::string(body->view()));
  return FilterDataStatus::Continue;
}


FilterHeadersStatus AddHeaderContext::onResponseHeaders(uint32_t) {
  LOG_INFO(std::string("onResponseHeaders ") + std::to_string(id()));
  addResponseHeader(root_->header_name_, root_->header_value_);
  replaceResponseHeader("location", "envoy-wasm");
  addResponseHeader("hello", "world");
  return FilterHeadersStatus::Continue;
}

void AddHeaderContext::onDone() { LOG_INFO(std::string("onDone " + std::to_string(id()))); }

void AddHeaderContext::onLog() { LOG_INFO(std::string("onLog " + std::to_string(id()))); }

void AddHeaderContext::onDelete() { LOG_INFO(std::string("onDelete " + std::to_string(id()))); }
