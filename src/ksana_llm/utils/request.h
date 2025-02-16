/* Copyright 2024 Tencent Inc.  All rights reserved.

==============================================================================*/
#pragma once

#include <sys/stat.h>
#include <vector>

#include "ksana_llm/utils/id_generator.h"
#include "ksana_llm/utils/status.h"
#include "ksana_llm/utils/tensor.h"
#include "ksana_llm/utils/waiter.h"

namespace ksana_llm {

struct SamplingConfig {
    int topk = 1;
    int num_beams = 1;
    int num_return_sequences = 1;
    float topp = 0.0f;
    float temperature = 0.0f;

    // The parameter for repetition penalty. 1.0 means no penalty
    float repetition_penalty = 1.0f;
    float length_penalty = 1.0f;

    // Tokens that stop the generation when they are generated.
    // The returned tokens will contain the stop tokens.
    std::vector<int> stop_token_ids;

    // The maximum numbers of tokens to generate, ignoring the number of tokens in the prompt.
    int max_new_tokens = 1024;

    int logprobs_num = 0;
};

typedef std::tuple<std::vector<int>, std::vector<std::vector<std::pair<int, float>>>, float> OutputTuple;

class Request {
  public:
    Request(const SamplingConfig& sampling_config);

    // The unique id of a request.
    int64_t req_id;

    // TODO(zakwang): Replace req_id
    std::vector<int64_t> req_ids;

    // The requested model name.
    std::string model_name;

    // The tokens of this request.
    std::vector<int> input_tokens;

    // TODO(zakwang): Replace output_tokens
    std::vector<OutputTuple> output_group;

    // The output tokens of this request.
    std::vector<int>& output_tokens;

    // The padded token num.
    int padded_size = 0;

    // Store token and their corresponding float probability values.
    std::vector<std::vector<std::pair<int, float>>>& logprobs;

    // The config of sampling.
    SamplingConfig sampling_config;

    // The waiter notified when request finished.
    std::shared_ptr<Waiter> waiter = nullptr;

    // The waiter notified when step finished.
    std::shared_ptr<Waiter> step_waiter = nullptr;

    // TODO(zakwang): Replace finished
    std::deque<bool> finisheds;

    // Whether the request is finished.
    bool& finished;

    // The finish statu of this request.
    Status finish_status;

    // Protect parallel access for output token.
    std::mutex output_mutex;

  private:
    // The id generator
    static IdGenerator id_generator_;
};

}  // namespace ksana_llm
