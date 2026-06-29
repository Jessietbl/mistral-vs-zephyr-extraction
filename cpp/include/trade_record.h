#pragma once

#include <optional>
#include <string>

struct TradeRecord {
    std::optional<double> exports;
    std::optional<double> imports;
    std::optional<double> trade_balance;
    std::optional<double> total_trade;

    // Format: YYYY-MM-DD
    std::string source_date;

    // Format: YYYY-MM-DD
    std::string evaluation_date;
};
