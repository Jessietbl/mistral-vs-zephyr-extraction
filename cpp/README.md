# Factuality and Freshness Validator — C++ Backend Prototype

This module demonstrates how selected quality checks from the Python offline LLM evaluation pipeline can be converted into deterministic backend validation logic.

The Python pipeline evaluates model outputs, benchmarks extraction quality, and performs loss analysis.

This C++ module focuses on production-style checks:

- required field validation
- arithmetic consistency
- unit plausibility
- stale source detection
- high-risk factuality failure detection

## Why C++

The target production environment may require low-latency backend components. This prototype shows how model-quality findings can be converted into fast, testable, deterministic validation logic.

## Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
