PROMPT_VARIANTS = {
    "strict_json": """
Extract the latest month trade statistics.

Required fields:
- exports
- imports
- trade_balance
- total_trade

Return JSON only.

Units must be RM billion.
"""
}