#include <iostream>
#include <string>

#include "freshness_policy.h"
#include "trade_record.h"
#include "validation_result.h"
#include "validator.h"

namespace {

int g_failures = 0;

void expect(bool condition, const std::string& description) {
    if (!condition) {
        std::cerr << "FAILED: " << description << "\n";
        ++g_failures;
    } else {
        std::cout << "ok - " << description << "\n";
    }
}

bool hasIssue(const ValidationResult& result, const std::string& code) {
    for (const auto& issue : result.issues) {
        if (issue.code == code) {
            return true;
        }
    }
    return false;
}

}  // namespace

void testValidRecordIsAccepted() {
    TradeRecord record;
    record.exports = 100.0;
    record.imports = 80.0;
    record.total_trade = 180.0;
    record.trade_balance = 20.0;
    record.source_date = "2026-06-01";
    record.evaluation_date = "2026-06-15";

    const TradeRecordValidator validator;
    const ValidationResult result = validator.validate(record);

    expect(result.accepted, "a fully consistent, fresh record is accepted");
    expect(result.issues.empty(), "a fully consistent, fresh record has no issues");
}

void testMissingRequiredFieldsRejected() {
    TradeRecord record;
    record.source_date = "2026-06-01";
    record.evaluation_date = "2026-06-15";

    const TradeRecordValidator validator;
    const ValidationResult result = validator.validate(record);

    expect(!result.accepted, "a record missing exports/imports is rejected");
    expect(hasIssue(result, "missing_exports"), "missing_exports is flagged");
    expect(hasIssue(result, "missing_imports"), "missing_imports is flagged");
}

void testNegativeValueRejected() {
    TradeRecord record;
    record.exports = 100.0;
    record.imports = -5.0;
    record.total_trade = 95.0;
    record.source_date = "2026-06-01";
    record.evaluation_date = "2026-06-15";

    const TradeRecordValidator validator;
    const ValidationResult result = validator.validate(record);

    expect(!result.accepted, "a negative imports value is rejected");
    expect(hasIssue(result, "negative_imports"), "negative_imports issue is present");
}

void testArithmeticInconsistencyDetected() {
    TradeRecord record;
    record.exports = 100.0;
    record.imports = 80.0;
    record.total_trade = 500.0;  // does not equal exports + imports
    record.source_date = "2026-06-01";
    record.evaluation_date = "2026-06-15";

    const TradeRecordValidator validator;
    const ValidationResult result = validator.validate(record);

    expect(!result.accepted, "exports + imports mismatch with total_trade is rejected");
    expect(hasIssue(result, "total_trade_inconsistent"), "total_trade_inconsistent issue is present");
}

void testUnitNormalizationFailureDetected() {
    TradeRecord record;
    record.exports = 100000.0;  // e.g. RM million mis-parsed at RM billion scale
    record.imports = 80.0;
    record.total_trade = 100080.0;
    record.source_date = "2026-06-01";
    record.evaluation_date = "2026-06-15";

    const TradeRecordValidator validator;
    const ValidationResult result = validator.validate(record);

    expect(!result.accepted, "an implausibly large exports value is rejected");
    expect(hasIssue(result, "implausible_exports"), "implausible_exports issue is present");
}

void testStaleSourceRejected() {
    TradeRecord record;
    record.exports = 100.0;
    record.imports = 80.0;
    record.total_trade = 180.0;
    record.source_date = "2026-01-01";
    record.evaluation_date = "2026-06-15";  // 165 days later, exceeds default 90-day policy

    const TradeRecordValidator validator;
    const ValidationResult result = validator.validate(record);

    expect(!result.accepted, "a source older than the freshness threshold is rejected");
    expect(hasIssue(result, "stale_source"), "stale_source issue is present");
}

void testFreshnessBoundaryAtExactlyMaxAge() {
    TradeRecord record;
    record.exports = 100.0;
    record.imports = 80.0;
    record.total_trade = 180.0;
    record.source_date = "2026-03-01";
    record.evaluation_date = "2026-05-30";  // exactly 90 days later

    const TradeRecordValidator validator;
    const ValidationResult result = validator.validate(record);

    expect(!hasIssue(result, "stale_source"), "a source exactly at the max-age boundary is not flagged stale");
}

void testFreshnessBoundaryOneDayOverMaxAge() {
    TradeRecord record;
    record.exports = 100.0;
    record.imports = 80.0;
    record.total_trade = 180.0;
    record.source_date = "2026-03-01";
    record.evaluation_date = "2026-05-31";  // 91 days later

    const TradeRecordValidator validator;
    const ValidationResult result = validator.validate(record);

    expect(hasIssue(result, "stale_source"), "a source one day past the max-age boundary is flagged stale");
}

void testFutureDatedSourceRejected() {
    TradeRecord record;
    record.exports = 100.0;
    record.imports = 80.0;
    record.total_trade = 180.0;
    record.source_date = "2026-07-01";
    record.evaluation_date = "2026-06-15";  // evaluation date precedes source date

    const TradeRecordValidator validator;
    const ValidationResult result = validator.validate(record);

    expect(!result.accepted, "a future-dated source is rejected");
    expect(hasIssue(result, "future_dated_source"), "future_dated_source issue is present");
}

void testConfigurableFreshnessPolicy() {
    TradeRecord record;
    record.exports = 100.0;
    record.imports = 80.0;
    record.total_trade = 180.0;
    record.source_date = "2026-01-01";
    record.evaluation_date = "2026-06-15";  // 165 days later

    FreshnessPolicy lenient_policy;
    lenient_policy.maximum_age_days = 365;

    const TradeRecordValidator validator(lenient_policy);
    const ValidationResult result = validator.validate(record);

    expect(!hasIssue(result, "stale_source"),
           "a lenient freshness policy accepts a source the default policy would reject");
}

void testDaysBetweenDatesIsCalendarCorrect() {
    const auto days = daysBetweenDates("2026-01-01", "2026-03-01");
    expect(days.has_value() && *days == 59,
           "daysBetweenDates computes calendar-correct day counts (Jan 1 -> Mar 1, 2026 = 59 days)");
}

void testUnparseableDateReturnsNullopt() {
    const auto days = daysBetweenDates("2026-13-01", "2026-06-15");
    expect(!days.has_value(), "an invalid month is rejected by the date parser rather than silently miscomputed");
}

int main() {
    testValidRecordIsAccepted();
    testMissingRequiredFieldsRejected();
    testNegativeValueRejected();
    testArithmeticInconsistencyDetected();
    testUnitNormalizationFailureDetected();
    testStaleSourceRejected();
    testFreshnessBoundaryAtExactlyMaxAge();
    testFreshnessBoundaryOneDayOverMaxAge();
    testFutureDatedSourceRejected();
    testConfigurableFreshnessPolicy();
    testDaysBetweenDatesIsCalendarCorrect();
    testUnparseableDateReturnsNullopt();

    if (g_failures > 0) {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1;
    }

    std::cout << "All tests passed.\n";
    return 0;
}
