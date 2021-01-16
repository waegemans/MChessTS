#pragma once

namespace chess {
struct Clock {
    int white_time_ms;
    int black_time_ms;
    int white_increment_ms;
    int black_increment_ms;
};
}