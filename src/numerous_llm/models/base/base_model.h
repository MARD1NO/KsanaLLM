/* Copyright 2023 Tencent Inc.  All rights reserved.

==============================================================================*/
#pragma once

#include "numerous_llm/utils/environment.h"
#include "numerous_llm/utils/status.h"

namespace numerous_llm {

class BaseModel {
 public:
  // Disable a default constructor
  BaseModel() {}

  virtual ~BaseModel() {}

  // The prefill stage.
  virtual Status ContextDecode() = 0;

  // The decode stage.
  virtual Status Decode() = 0;
};

}  // namespace numerous_llm