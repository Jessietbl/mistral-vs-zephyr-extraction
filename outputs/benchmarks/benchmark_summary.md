# Benchmark Summary

This benchmark compares Mistral-7B-Instruct and Zephyr-7B-Beta for structured extraction of Malaysian trade indicators from 60 PDF reports.

Values are reported in RM billion. Results below are raw extraction metrics before outlier filtering or quality-gating.

## Raw Benchmark Results

| Metric | Zephyr-7B | Mistral-7B | Better |
|---|---:|---:|---|
| Exports MAE | 9.90 | 20.07 | Zephyr-7B |
| Exports RMSE | 25.20 | 111.32 | Zephyr-7B |
| Imports MAE | 7.30 | 16.56 | Zephyr-7B |
| Imports RMSE | 21.57 | 93.33 | Zephyr-7B |
| Trade Balance MAE | 3.93e9 | 2.70e10 | Zephyr-7B |
| Trade Balance RMSE | 1.81e10 | 1.96e11 | Zephyr-7B |
| Total Trade MAE | 19.85 | 43.85 | Zephyr-7B |
| Total Trade RMSE | 51.74 | 205.66 | Zephyr-7B |

## Key Finding

Zephyr-7B outperformed Mistral-7B across the raw benchmark metrics.

However, both models produced severe outlier errors caused by unit-conversion and parsing failures, especially for trade balance values.

## Interpretation

The benchmark should be interpreted as a pipeline-level comparison rather than a pure model-only comparison. Differences may reflect both model behaviour and extraction-pipeline handling, including JSON parsing, fallback behaviour, and unit normalization.

## Product Lesson

LLM extraction should not be consumed directly without validation. A reliable workflow requires:

- JSON validation
- field completion checks
- unit normalization
- logical consistency checks
- outlier detection
- confidence-based human review routing
