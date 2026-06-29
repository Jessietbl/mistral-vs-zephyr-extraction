#pragma once

#include <string>
#include <vector>

enum class RiskLevel {
    Low,
    Medium,
    High
};

struct ValidationIssue {
    std::string code;
    std::string message;
    RiskLevel risk_level;
};

struct ValidationResult {
    bool accepted;
    std::vector<ValidationIssue> issues;
};
