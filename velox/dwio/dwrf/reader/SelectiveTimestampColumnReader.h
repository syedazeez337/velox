/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "velox/dwio/common/SelectiveColumnReaderInternal.h"
#include "velox/dwio/dwrf/common/DecoderUtil.h"
#include "velox/dwio/dwrf/reader/DwrfData.h"

namespace facebook::velox::dwrf {
class SelectiveTimestampColumnReader
    : public dwio::common::SelectiveColumnReader {
 public:
  // The readers produce int64_t, the vector is Timestamps.
  using ValueType = int64_t;

  SelectiveTimestampColumnReader(
      const std::shared_ptr<const dwio::common::TypeWithId>& fileType,
      DwrfParams& params,
      common::ScanSpec& scanSpec);

  void seekToRowGroup(int64_t index) override;
  uint64_t skip(uint64_t numValues) override;

  void read(int64_t offset, const RowSet& rows, const uint64_t* incomingNulls)
      override;

  void getValues(const RowSet& rows, VectorPtr* result) override;

 private:
  template <bool isDense>
  void readHelper(const common::Filter* filter, const RowSet& rows);

  void
  processNulls(const bool isNull, const RowSet& rows, const uint64_t* rawNulls);
  void processFilter(
      const common::Filter* filter,
      const RowSet& rows,
      const uint64_t* rawNulls);

  const TimestampPrecision precision_;

  std::unique_ptr<dwio::common::IntDecoder</*isSigned*/ true>> seconds_;
  std::unique_ptr<dwio::common::IntDecoder</*isSigned*/ false>> nano_;

  // Values from copied from 'seconds_'. Nanos are in 'values_'.
  BufferPtr secondsValues_;
  RleVersion version_;
};

} // namespace facebook::velox::dwrf
