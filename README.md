# LLM Model Quality Evaluation for Structured Trade Data Extraction

## Overview

This project evaluates open-source 7B-parameter instruction models for extracting structured economic indicators from noisy Malaysian trade statistics PDF reports.

The project is framed as a model-quality evaluation workflow: PDF ingestion, OCR-aware text extraction, prompt-based structured extraction, JSON validation, numeric consistency checks, benchmark evaluation, and executive model-selection recommendations.

## Executive Summary

This project evaluates whether lightweight open-source 7B instruction models can reliably extract structured economic indicators from noisy PDF reports.

The evaluation focuses on:

* structured output reliability
* numeric extraction accuracy
* robustness to OCR noise
* logical consistency of extracted values
* suitability for downstream analytics workflows

The project demonstrates a practical model-quality evaluation workflow similar to those used when assessing LLM-powered product features before deployment.


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
│
├── data/
│
├── inputs/
│
├── notebooks/
│   ├── 01_llm_mistral_7b_clean.ipynb
│   └── 01_llm_zephyr_7b_clean.ipynb
│
├── outputs/
│   ├── predictions/
│   ├── benchmarks/
│   └── figures/
│
├── reports/
│   ├── error_analysis.md
│   └── executive_decision_memo.md
│
└── src/
    ├── prompts.py
    ├── parsing.py
    └── validation.py
```

### Repository Components

**notebooks/**

Contains reproducible benchmark notebooks for evaluating Mistral-7B and Zephyr-7B on structured trade-data extraction tasks.

**src/**

Reusable utility modules for:

* prompt templates
* JSON parsing
* output validation
* numeric consistency checking

**reports/**

Contains benchmark interpretation, error analysis, and executive model-selection recommendations.

**outputs/**

Stores generated predictions, benchmark metrics, and visualization outputs.

**inputs/**

Contains sample PDF reports used for public demonstrations.

**data/**

Contains sample ground-truth data and supporting documentation.

```
```

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

## How to Run

Install dependencies:

```bash
pip install -r requirements.txt
```

Open and execute either notebook:

```text
notebooks/01_llm_mistral_7b_clean.ipynb
```

or

```text
notebooks/01_llm_zephyr_7b_clean.ipynb
```

The notebooks perform:

1. PDF text extraction
2. OCR fallback processing
3. Prompt-based structured extraction
4. JSON parsing and validation
5. Benchmark comparison against sample ground-truth data

```
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

