# LLM Model Quality Evaluation for Structured Trade Data Extraction

## Overview

This project evaluates open-source 7B-parameter instruction models for extracting structured economic indicators from noisy Malaysian trade statistics PDF reports.

The project is framed as a model-quality evaluation workflow: PDF ingestion, OCR-aware text extraction, prompt-based structured extraction, JSON validation, numeric consistency checks, benchmark evaluation, and executive model-selection recommendations.

## Models Evaluated

* Mistral-7B-Instruct
* Zephyr-7B-Beta

## Extraction Task

The models are asked to extract the latest monthly trade indicators from each bulletin and return valid JSON.

| Field         | Description               | Unit       |
| ------------- | ------------------------- | ---------- |
| exports       | Monthly exports           | RM billion |
| imports       | Monthly imports           | RM billion |
| trade_balance | Monthly trade balance     | RM billion |
| total_trade   | Monthly total trade value | RM billion |

Expected output format:

```json
{
  "exports": 129.4,
  "imports": 112.8,
  "trade_balance": 16.6,
  "total_trade": 242.2
}
```

## Why This Project Matters

For user-facing GenAI products, model quality is not only about producing fluent text. Reliable structured extraction requires:

* valid machine-readable output
* low numeric error
* consistent unit handling
* robustness to noisy OCR text
* detection of malformed or incomplete responses
* clear recommendation logic for model selection

This project demonstrates how LLM output quality can be evaluated systematically before being used in a product or analytics workflow.

## Evaluation Design

The benchmark compares model outputs against manually verified ground-truth trade statistics.

Metrics include:

* JSON validity rate
* field completion rate
* MAE
* RMSE
* MAPE
* tolerance accuracy
* logical consistency pass rate
* missing-field rate
* error type distribution

## Experimental Pipeline

```text
PDF Reports
      ↓
Text Extraction
      ↓
OCR Fallback
      ↓
Text Cleaning
      ↓
Prompt-Based LLM Extraction
      ↓
JSON Parsing
      ↓
Unit Normalization
      ↓
Logical Validation
      ↓
Ground-Truth Benchmarking
      ↓
Error Analysis
      ↓
Model Selection Recommendation
```

## Repository Structure

```text
mistral-vs-zephyr-extraction/
├── README.md
├── requirements.txt
├── data/
│   ├── sample_ground_truth.csv
│   └── data_dictionary.md
├── inputs/
│   └── sample_pdfs/
├── notebooks/
│   ├── 01_run_extraction.ipynb
│   └── 02_benchmark_analysis.ipynb
├── outputs/
│   ├── predictions/
│   ├── benchmarks/
│   └── figures/
├── reports/
│   ├── model_quality_report.md
│   ├── error_analysis.md
│   └── executive_decision_memo.md
└── src/
    ├── config.py
    ├── text_processing.py
    ├── prompts.py
    ├── parsing.py
    ├── validation.py
    ├── llm_loader.py
    ├── run_extraction.py
    └── run_evaluation.py
```

## Benchmark Results

After running the public sample benchmark, update this table with actual values.

| Metric                        |    Mistral-7B |     Zephyr-7B | Better        |
| ----------------------------- | ------------: | ------------: | ------------- |
| JSON validity rate            | INSERT_RESULT | INSERT_RESULT | INSERT_RESULT |
| Field completion rate         | INSERT_RESULT | INSERT_RESULT | INSERT_RESULT |
| Logical consistency pass rate | INSERT_RESULT | INSERT_RESULT | INSERT_RESULT |
| Average MAE                   | INSERT_RESULT | INSERT_RESULT | INSERT_RESULT |
| Average RMSE                  | INSERT_RESULT | INSERT_RESULT | INSERT_RESULT |
| Average MAPE                  | INSERT_RESULT | INSERT_RESULT | INSERT_RESULT |

## Error Analysis

The benchmark tracks common LLM extraction failure modes:

| Error Type            | Description                                                      | Product Risk                 |
| --------------------- | ---------------------------------------------------------------- | ---------------------------- |
| Wrong month selected  | Model extracts previous-month comparison instead of latest month | Incorrect user-facing answer |
| Unit confusion        | RM million interpreted as RM billion                             | Large numeric error          |
| Missing field         | Model omits exports, imports, balance, or total trade            | Incomplete structured output |
| Invalid JSON          | Model returns explanation instead of parseable JSON              | Pipeline failure             |
| Logical inconsistency | Exports + imports does not match total trade                     | Low trust in answer          |
| OCR corruption        | PDF text extraction corrupts currency or numeric values          | Wrong downstream extraction  |

## Model Selection Framing

The final model recommendation is based on the product priority:

* choose the model with lower numeric error when factual precision matters most
* choose the model with higher JSON validity when pipeline reliability matters most
* use logical validation and confidence scoring to route uncertain outputs for human review

## How to Run

Install dependencies:

```bash
pip install -r requirements.txt
```

Run extraction:

```bash
python -m src.run_extraction --model mistral_7b --prompt strict_json
python -m src.run_extraction --model zephyr_7b --prompt strict_json
```

Run evaluation:

```bash
python -m src.run_evaluation
```

Expected outputs:

```text
outputs/predictions/mistral_7b_strict_json_predictions.csv
outputs/predictions/zephyr_7b_strict_json_predictions.csv
outputs/benchmarks/benchmark_summary.csv
outputs/figures/model_mae_comparison.png
reports/model_quality_report.md
```

## Limitations

* The public version uses sample PDFs and sample ground-truth values only.
* Full thesis data and private experiments are excluded.
* OCR quality materially affects extraction accuracy.
* This project demonstrates model evaluation, not production deployment.
* Larger-scale evaluation would be needed before production use.

## Future Work

* prompt A/B testing
* few-shot prompting
* confidence calibration
* OCR quality scoring
* hybrid regex + LLM extraction
* additional models such as Gemma, Llama, Phi, and Qwen
* human-review routing for low-confidence outputs
* multilingual trade bulletin extraction

