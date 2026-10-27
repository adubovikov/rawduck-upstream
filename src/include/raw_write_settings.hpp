#pragma once

#include "duckdb.hpp"

namespace duckdb {

struct RawWriteSettings {
	static constexpr idx_t DEFAULT_POOL_MIN_ROWS = 4096;
	static constexpr idx_t DEFAULT_PIPELINE_DEPTH = 4;
	static constexpr idx_t MAX_POOL_THREADS = 16;
	static constexpr idx_t MAX_PIPELINE_THREADS = 16;
	// Auto (rawduck_pool_threads = 0) append-pool size cap. Each pool worker owns
	// an optimistic writer; above ~4 concurrent writers, a bulk import leaves
	// free blocks that CHECKPOINT does not reclaim (the file grows 1.5-1.9x for
	// the same data). Explicit rawduck_pool_threads still goes up to
	// MAX_POOL_THREADS.
	static constexpr idx_t AUTO_POOL_THREAD_CAP = 4;

	idx_t pool_min_rows = DEFAULT_POOL_MIN_ROWS;
	idx_t pool_threads = 0;
	idx_t pipeline_threads = 0;
	idx_t pipeline_consumers = 0;
	idx_t pipeline_depth = DEFAULT_PIPELINE_DEPTH;
	bool overlap_flush_auto = false;
	// DuckDB's own `threads` setting; auto sizing never exceeds it
	idx_t engine_threads = 0;

	static RawWriteSettings Get(ClientContext &context);
	idx_t PoolThreadCount(idx_t batch_rows) const;
	idx_t PipelineThreadCount() const;
	idx_t PipelineConsumerCount() const;
	bool OverlapFlushForBatch(ClientContext &context, bool shape_absorbed, idx_t batch_rows) const;
};

} // namespace duckdb
