#include <gtest/gtest.h>

#include "eval/Score.hpp"

TEST(TestScore,relational) {
    std::vector<chess::Score> scores {};
    scores.emplace_back(true,-1);
    scores.emplace_back(true,-4);
    scores.emplace_back(-89);
    scores.emplace_back(-1);
    scores.emplace_back(0);
    scores.emplace_back(1);
    scores.emplace_back(76);
    scores.emplace_back(true,8);
    scores.emplace_back(true,1);

    for (unsigned i = 0; i < scores.size(); i++) {
        auto a = scores[i];
        for (unsigned j = i+1; j < scores.size(); j++) {
            auto b = scores[j];
            EXPECT_LT(a,b);
            EXPECT_GT(b,a);
        }
    }
}
