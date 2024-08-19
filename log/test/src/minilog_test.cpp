
#include <gtest/gtest.h>
#include <unistd.h>

#include "test_log.h"


size_t test_log_get_file_log_cur_size(stLogFile_t *file_log)
{
    return file_log->cur_file_size;
}

TEST(minilog_test, log_fifo_sync2file_time)
{
    stLogFile_t *file_log = nullptr;

    EXPECT_GT(log_system("gtest start"), 0) <<  "log system can't log to file";

    file_log = _log_get_log_file(0);
    ASSERT_NE(file_log, nullptr);

    EXPECT_EQ(test_log_get_file_log_cur_size(file_log), 0) << "FIFO less wait time, already sync to log file";
    usleep(15000);
    EXPECT_GT(test_log_get_file_log_cur_size(file_log), 0) << "FIFO more than wait time, not sync to log file";
}

TEST(minilog_test, log_debug_level_output)
{
    EXPECT_EQ(log_debug("="), 0) << "log debug can log to file";
    usleep(100000);
}

TEST(minilog_test, log_info_level_output)
{
    EXPECT_EQ(log_info("-"), 0) << "log info can log to file";
    usleep(100000);
}

TEST(minilog_test, log_warn_level_output)
{
    EXPECT_GT(log_warn("+"), 0) << "log warn can't log to file";
    usleep(100000);
}

TEST(minilog_test, log_error_level_output)
{
    EXPECT_GT(log_error("x"), 0) << "log error can't log to file";
    usleep(100000);
}

TEST(minilog_test, log_system_level_output)
{
    EXPECT_GT(log_system("#"), 0) <<  "log system can't log to file";
    usleep(100000);
}

