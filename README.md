# mistral-vs-zephyr-extraction
Mistral vs Zephyr: LLM-Based Trade Data Extraction from PDF Reports
Overview

This project evaluates the performance of open-source Large Language Models (LLMs) for extracting structured economic indicators from Malaysian trade statistics bulletins published as PDF documents.

The pipeline combines PDF text extraction, OCR fallback, prompt-based information extraction, and automated benchmarking against ground-truth data to assess model performance.

Research Objective

The study compares:

Mistral-7B
Zephyr-7B

for extracting key monthly trade indicators from unstructured government reports.

Extracted fields include:

Field	Description
Exports	Monthly exports (RM billion)
Imports	Monthly imports (RM billion)
Trade Balance	Monthly trade balance (RM billion)
Total Trade	Monthly total trade value (RM billion)
Methodology
1. PDF Processing

Trade bulletins are extracted using:

pdfplumber
OCR fallback using Tesseract

Text preprocessing includes:

OCR correction
Currency normalization
Number formatting cleanup
Noise reduction
2. Prompt-Based Information Extraction

The LLM receives:

Bulletin text
Structured extraction instructions
Unit conversion rules
JSON output constraints

The model returns:

{
  "exports": 129.4,
  "imports": 112.8,
  "trade_balance": 16.6,
  "total_trade": 242.2
}
3. Benchmark Evaluation

Predictions are compared against manually verified ground-truth values.

Metrics include:

Mean Absolute Error (MAE)
Root Mean Squared Error (RMSE)
R² Score
Technology Stack
Python
Google Colab
Hugging Face Transformers
BitsAndBytes (4-bit quantization)
PyTorch
pdfplumber
pytesseract
pandas
scikit-learn
matplotlib
Repository Structure
mistral-vs-zephyr-extraction/
│
├── notebooks/
│   ├── mistral_extraction.ipynb
│   └── zephyr_extraction.ipynb
│
├── data/
│   ├── pdfs/
│   └── ground_truth/
│
├── results/
│   ├── predictions/
│   ├── benchmarks/
│   └── visualizations/
│
├── README.md
└── requirements.txt
Experimental Pipeline
PDF Reports
      ↓
Text Extraction
      ↓
OCR Fallback
      ↓
Text Cleaning
      ↓
LLM Prompting
      ↓
JSON Extraction
      ↓
Ground Truth Comparison
      ↓
Benchmark Metrics
      ↓
Visualization & Analysis
Results
Metric	Mistral-7B	Zephyr-7B
MAE	TBD	TBD
RMSE	TBD	TBD
R²	TBD	TBD
JSON Validity Rate	TBD	TBD
Key Contributions
Developed an automated PDF-to-JSON extraction pipeline.
Applied LLMs to real-world economic reports.
Implemented benchmarking against verified trade statistics.
Evaluated extraction quality using standard regression metrics.
Investigated the effectiveness of lightweight 7B-parameter models in information extraction tasks.
Future Work
Few-shot prompting
Retrieval-Augmented Generation (RAG)
Additional open-source LLMs (Llama, Phi, Gemma)
Fine-tuning on economic reports
Multi-country trade bulletin extraction
