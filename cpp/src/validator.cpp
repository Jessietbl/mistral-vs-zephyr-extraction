#include "validator.h"

#include <cmath>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace {
constexpr double kTolerance = 1.0;          // RM billion tolerance
constexpr double kMaxReasonableTrade = 1000.0; // RM billion
constexpr int kMaxFreshnessDays = 90;

void addIssue(
    ValidationResult& result,
    const std::string& code,
    const std::string& message,
    RiskLevel risk_level
) {
    result.issues.push_back({code, message, risk_level});
}
}

ValidationResult TradeRecordValidator::validate(const TradeRecord& record) const {
    ValidationResult result;
    result.accepted = true;

    checkRequiredFields(record, result);
    checkNonNegativeValues(record, result);
    checkPlausibleRange(record, result);
    checkArithmeticConsistency(record, result);
    checkFreshness(record, result);

    for (const auto& issue : result.issues) {
        if (issue.risk_level == RiskLevel::High) {
            result.accepted = false;
            break;
        }
    }

    return result;
}

void TradeRecordValidator::checkRequiredFields(
    const TradeRecord& record,
    ValidationResult& result
) const {
    if (!record.exports.has_value()) {
        addIssue(result, "missing_exports", "Exports value is missing.", RiskLevel::High);
    }

    if (!record.imports.has_value()) {
        addIssue(result, "missing_imports", "Imports value is missing.", RiskLevel::High);
    }

    if (!record.total_trade.has_value()) {
        addIssue(result, "missing_total_trade", "Total trade value is missing.", RiskLevel::Medium);
    }

    if (record.source_date.empty()) {
        addIssue(result, "missing_source_date", "Source document date is missing.", RiskLevel::High);
    }
}

void TradeRecordValidator::checkNonNegativeValues(
    const TradeRecord& record,
    ValidationResult& result
) const {
    const std::vector<std::pair<std::string, std::optional<double>>> fields = {
        {"exports", record.exports},
        {"imports", record.imports},
        {"total_trade", record.total_trade}
    };

    for (const auto& [name, value] : fields) {
        if (value.has_value() && value.value() < 0.0) {
            addIssue(
                result,
                "negative_" + name,
                name + " cannot be negative.",
                RiskLevel::High
            );
        }
    }
}

void TradeRecordValidator::checkPlausibleRange(
    const TradeRecord& record,
    ValidationResult& result
) const {
    const std::vector<std::pair<std::string, std::optional<double>>> fields = {
        {"exports", record.exports},
        {"imports", record.imports},
        {"total_trade", record.total_trade}
    };

    for (const auto& [name, value] : fields) {
        if (value.has_value() && value.value() > kMaxReasonableTrade) {
            addIssue(
                result,
                "implausible_" + name,
                name + " is outside the expected RM billion range. Possible unit-normalization failure.",
                RiskLevel::High
            );
        }
    }
}

void TradeRecordValidator::checkArithmeticConsistency(
    const TradeRecord& record,
    ValidationResult& result
) const {
    if (record.exports.has_value() &&
        record.imports.has_value() &&
        record.total_trade.has_value()) {

        const double expected_total = record.exports.value() + record.imports.value();

        if (std::abs(expected_total - record.total_trade.value()) > kTolerance) {
            addIssue(
                result,
                "total_trade_inconsistent",
                "exports + imports does not match total_trade.",
                RiskLevel::High
            );
        }
    }

    if (record.exports.has_value() &&
        record.imports.has_value() &&
        record.trade_balance.has_value()) {

        const double expected_balance = record.exports.value() - record.imports.value();

        if (std::abs(expected_balance - record.trade_balance.value()) > kTolerance) {
            addIssue(
                result,
                "trade_balance_inconsistent",
                "exports - imports does not match trade_balance.",
                RiskLevel::High
            );
        }
    }
}

void TradeRecordValidator::checkFreshness(
    const TradeRecord& record,
    ValidationResult& result
) const {
    if (record.source_date.empty() || record.evaluation_date.empty()) {
        return;
    }

    const int age_days = daysBetween(record.source_date, record.evaluation_date);

    if (age_days > kMaxFreshnessDays) {
        addIssue(
            result,
            "stale_source",
            "Source document is older than the accepted freshness threshold.",
            RiskLevel::High
        );
    }
}

int TradeRecordValidator::daysBetween(
    const std::string& start_date,
    const std::string& end_date
) const {
    std::tm start = {};
    std::tm end = {};

    std::istringstream start_stream(start_date);
    std::istringstream end_stream(end_date);

    start_stream >> std::get_time(&start, "%Y-%m-%d");
    end_stream >> std::get_time(&end, "%Y-%m-%d");

    if (start_stream.fail() || end_stream.fail()) {
        return 0;
    }

    const std::time_t start_time = std::mktime(&start);
    const std::time_t end_time = std::mktime(&end);

    const double seconds = std::difftime(end_time, start_time);
    return static_cast<int>(seconds / (60 * 60 * 24));
}
