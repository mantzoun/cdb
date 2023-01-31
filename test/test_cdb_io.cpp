#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

#include "cdb_io.h"

TEST_GROUP(CDB_IO_TestGroup)
{
};

TEST(CDB_IO_TestGroup, TestStartStopFifoRead)
{
    cdb::IO cdb_io;
    cdb::Logger logger(cdb::CDB_LOG_DEBUG);

    cdb_io.set_logger(&logger);
    CHECK(true == cdb_io.fifo_init("/tmp/no_fifo"));

    CHECK(true == cdb_io.fifo_terminate());
}

TEST(CDB_IO_TestGroup, TestDuplicateStartFifoRead)
{
    cdb::IO cdb_io;
    cdb::Logger logger(cdb::CDB_LOG_DEBUG);

    cdb_io.set_logger(&logger);
    CHECK(true == cdb_io.fifo_init("/tmp/no_fifo"));
    CHECK(false == cdb_io.fifo_init("/tmp/no_fifo"));

    CHECK(true == cdb_io.fifo_terminate());
}

TEST(CDB_IO_TestGroup, TestDuplicateStopFifoRead)
{
    cdb::IO cdb_io;
    cdb::Logger logger(cdb::CDB_LOG_DEBUG);

    cdb_io.set_logger(&logger);
    CHECK(true == cdb_io.fifo_init("/tmp/no_fifo"));

    CHECK(true == cdb_io.fifo_terminate());
    CHECK(false == cdb_io.fifo_terminate());
}
int main(int ac, char** av)
{
       return CommandLineTestRunner::RunAllTests(ac, av);
}
