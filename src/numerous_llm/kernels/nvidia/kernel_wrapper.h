/* Copyright 2023 Tencent Inc.  All rights reserved.

==============================================================================*/

#include <cublasLt.h>
#include <cublas_v2.h>
#include <cuda_runtime.h>
namespace numerous_llm {

void LookupEmbedding(const void* ids, const void* offset, const void* emb, const void* pos, void* output,
                     int vocab_size, int hidden_size, int bs, int step, int vocab_id, cudaStream_t stream);

void InvokeLayerNorm(const void* input, const void* weight, const float layernorm_eps, const int m, const int n,
                     void* output, cudaStream_t stream);

void InvokeMatMul(cublasHandle_t cublas_handle, cublasLtHandle_t cublaslt_handle, int m, int n, int k,
                  const void* a_ptr, const void* b_ptr, void* c_ptr, cudaStream_t& stream);

void InvokeAddBiasResidual(const void* input, const void* bias, const int m, const int n, void* output,
                           cudaStream_t stream);

void InvokeSiluActivation(const void* input, const void* bias, const int m, const int n, void* output,
                          cudaStream_t stream);

void AssembleLastToken(const void* input, const void* offset, const int batch_size, const int hidden_units_num,
                       void* output, cudaStream_t& stream);

void AttenVarlen(void* q, void* k, void* v, void* out, void* seqlen, int total_tokens, int max_tokens, int batch,
                 int num_heads, int head_size, bool is_causal, int rank, cudaStream_t stream);
}  // namespace numerous_llm