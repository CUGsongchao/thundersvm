//
// Created by jiashuai on 17-9-15.
//
#include <thundersvm/syncdata.h>
#include "gtest/gtest.h"

INITIALIZE_EASYLOGGINGPP
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
//    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime %level %fbase:%line : %msg");
    el::Loggers::addFlag(el::LoggingFlag::FixedTimeFormat);
    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    //cudaSetDevice(1);
    return RUN_ALL_TESTS();
}
