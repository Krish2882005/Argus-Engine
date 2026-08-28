# Argus Engine

Argus Engine is being built as a zero-copy data ingestion pipeline designed for real-time processing of high-volume data streams. It will use shared memory and a lock-free queue to route data between processes without any dynamic memory allocations on the hot path. The compute layer will apply SIMD vectorization to process the data with deterministic, sub-millisecond latency.
