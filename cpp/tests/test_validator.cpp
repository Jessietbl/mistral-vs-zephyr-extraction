#include "validator.h"

#include <cassert>
#include <iostream>

void testValidRecordPasses() {
    TradeRecord record;
    record.exports = 120.0;
    record.imports = 100.0;
    record.trade_balance = 20.0;
    record.total_trade = 220.0;
    record.source_date = "2024-05-01";
    record.evaluation_date = "2024-06-01";

    TradeRecordValidator validator;
    ValidationResult result = validator.validate(record);

    assert(result.accepted);
    assert(result.issues.empty());
}

void testInconsistentTotalTradeFails() {
    TradeRecord record;
    record.exports = 120.0;
    record.imports = 100.0;
    record.trade_balance = 20.0;
    record.total_trade = 500.0;
    record.source_date = "2024-05-01";
    record.evaluation_date = "2024-06-01";

    TradeRecordValidator validator;
    ValidationResult result = validator.validate(record);

    assert(!result.accepted);
    assert(!result.issues.empty());
}

void testImplausibleUnitFails() {
    TradeRecord record;
    record.exports = 126000.0;
    record.imports = 100.0;
    record.trade_balance = 125900.0;
    record.total_trade = 126100.0;
    record.source_date = "2024-05-01";
    record.evaluation_date = "2024-06-01";

    TradeRecordValidator validator;
    ValidationResult result = validator.validate(record);

    assert(!result.accepted);
    assert(!result.issues.empty());
}

void testStaleSourceFails() {
    TradeRecord record;
    record.exports = 120.0;
    record.imports = 100.0;
    record.trade_balance = 20.0;
    record.total_trade = 220.0;
    record.source_date = "2023-01-01";
    record.evaluation_date = "2024-06-01";

    TradeRecordValidator validator;
    ValidationResult result = validator.validate(record);

    assert(!result.accepted);
    assert(!result.issues.empty());
}

int main() {
    testValidRecordPasses();
    testInconsistentTotalTradeFails();
    testImplausibleUnitFails();
    testStaleSourceFails();

    std::cout << "All validation tests passed.\n";
    return 0;
}
