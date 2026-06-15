def validate_trade_record(row):
    
    issues = []

    exports = row.get("exports")
    imports = row.get("imports")
    total_trade = row.get("total_trade")

    if (
        exports is not None
        and imports is not None
        and total_trade is not None
    ):

        if abs((exports + imports) - total_trade) > 1:
            issues.append("total_trade_inconsistent")

    return issues