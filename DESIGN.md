# System Design

## Current System

The current implementation evaluates structured trade-data extraction using
Mistral-7B and Zephyr-7B.

The Python pipeline performs:

- PDF text extraction
- OCR fallback
- prompt-based extraction
- JSON parsing
- unit normalization
- benchmark evaluation

The C++ validator performs:

- required-field validation
- numeric plausibility checks
- arithmetic consistency checks
- source freshness checks
- severity-based issue reporting

## Design Decisions

Deterministic validation is applied before more expensive learned evaluation
because rule-based checks are fast, explainable, reproducible, and effective
for detecting schema, arithmetic, unit, and temporal failures.

The current implementation uses trade statistics as a concrete evaluation
domain. The underlying validation concepts could later be extended to other
structured factual claims.

## Current Limitations

- The benchmark currently covers two open-source language models.
- The public evaluation dataset is relatively small.
- Freshness currently uses a fixed threshold.
- The implementation is demonstrated on structured trade statistics.
- Online experimentation and production-scale deployment are outside the
  current project scope.

## Future Roadmap

### Phase 1 — Reliability

- automated continuous integration
- expanded C++ and Python unit tests
- reproducible evaluation runs

### Phase 2 — Deeper Evaluation

- categorized failure analysis
- latest-period selection accuracy
- paired bootstrap confidence intervals
- quality-versus-coverage analysis

### Phase 3 — Generalization

- configurable freshness policies
- reusable structured-claim schemas
- evidence-grounding validation
- monitoring and quality dashboards
