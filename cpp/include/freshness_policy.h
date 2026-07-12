#pragma once

struct FreshnessPolicy {
    // Maximum permitted age of a source before it is considered stale.
    int maximum_age_days{90};

    // Reject source dates that occur after the evaluation date.
    bool reject_future_dates{true};
};
