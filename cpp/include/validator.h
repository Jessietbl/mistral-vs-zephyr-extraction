#pragma once

#include "trade_record.h"
#include "validation_result.h"

class TradeRecordValidator {
public:
    ValidationResult validate(const TradeRecord& record) const;

private:
    void checkRequiredFields(const TradeRecord& record, ValidationResult& result) const;
    void checkNonNegativeValues(const TradeRecord& record, ValidationResult& result) const;
    void checkPlausibleRange(const TradeRecord& record, ValidationResult& result) const;
    void checkArithmeticConsistency(const TradeRecord& record, ValidationResult& result) const;
    void checkFreshness(const TradeRecord& record, ValidationResult& result) const;

    int daysBetween(const std::string& start_date, const std::string& end_date) const;
};
