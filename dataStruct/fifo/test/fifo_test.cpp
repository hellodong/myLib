#include <gtest/gtest.h>
#include <cstring>
#include <pthread.h>
#include <atomic>
#include <vector>
#include "fifo.h"

class FifoTest : public ::testing::Test 
{
protected:
    struct _fifo_t fifo;
    unsigned char buffer[0x1000000];
    
    void SetUp() override {
        memset(&fifo, 0, sizeof(fifo));
        memset(buffer, 0, sizeof(buffer));
    }
};

TEST_F(FifoTest, InitWithValidBuffer) {
    int result = fifo_init(&fifo, buffer, 256);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(fifo.size, 256);
    EXPECT_EQ(fifo.in, 0);
    EXPECT_EQ(fifo.out, 0);
}

TEST_F(FifoTest, InitWithNullBuffer) {
    int result = fifo_init(&fifo, NULL, 256);
    EXPECT_EQ(result, -1);
}

TEST_F(FifoTest, InitWithNonPowerOfTwoSize) {
    int result = fifo_init(&fifo, buffer, 255);
    EXPECT_EQ(result, -1);
}

TEST_F(FifoTest, InitWithSizeOne) {
    int result = fifo_init(&fifo, buffer, 1);
    EXPECT_EQ(result, 0);
}

TEST_F(FifoTest, PutAndGetSingleByte) {
    fifo_init(&fifo, buffer, 256);
    
    char write_data = 'A';
    unsigned int written = fifo_put(&fifo, &write_data, 1);
    EXPECT_EQ(written, 1);
    
    char read_data = 0;
    unsigned int read = fifo_get(&fifo, &read_data, 1);
    EXPECT_EQ(read, 1);
    EXPECT_EQ(read_data, 'A');
}

TEST_F(FifoTest, PutMultipleBytes) {
    fifo_init(&fifo, buffer, 256);
    
    char write_data[] = "Hello";
    unsigned int written = fifo_put(&fifo, write_data, 5);
    EXPECT_EQ(written, 5);
}

TEST_F(FifoTest, GetMultipleBytes) {
    fifo_init(&fifo, buffer, 256);
    
    char write_data[] = "Hello";
    fifo_put(&fifo, write_data, 5);
    
    char read_data[10] = {0};
    unsigned int read = fifo_get(&fifo, read_data, 5);
    EXPECT_EQ(read, 5);
    EXPECT_STREQ(read_data, "Hello");
}

TEST_F(FifoTest, PutWhenFull) {
    fifo_init(&fifo, buffer, 256);
    
    char write_data[256];
    memset(write_data, 'X', 256);
    
    unsigned int written = fifo_put(&fifo, write_data, 256);
    EXPECT_EQ(written, 256);
    
    written = fifo_put(&fifo, write_data, 1);
    EXPECT_EQ(written, 0);
}

TEST_F(FifoTest, GetWhenEmpty) {
    fifo_init(&fifo, buffer, 256);
    
    char read_data[10];
    unsigned int read = fifo_get(&fifo, read_data, 10);
    EXPECT_EQ(read, 0);
}

TEST_F(FifoTest, WrapAroundPut) {
    fifo_init(&fifo, buffer, 16);
    
    char write_data[16];
    memset(write_data, 'A', 16);
    fifo_put(&fifo, write_data, 16);
    
    char read_data[16];
    fifo_get(&fifo, read_data, 16);
    
    memset(write_data, 'B', 8);
    unsigned int written = fifo_put(&fifo, write_data, 8);
    EXPECT_EQ(written, 8);
}

TEST_F(FifoTest, WrapAroundGet) {
    fifo_init(&fifo, buffer, 16);
    
    char write_data[16];
    memset(write_data, 'A', 16);
    fifo_put(&fifo, write_data, 16);
    
    char read_data[8];
    fifo_get(&fifo, read_data, 8);
    
    memset(write_data, 'B', 8);
    fifo_put(&fifo, write_data, 8);
    
    memset(read_data, 0, 8);
    unsigned int read = fifo_get(&fifo, read_data, 8);
    EXPECT_EQ(read, 8);
    EXPECT_EQ(read_data[0], 'A');
    EXPECT_EQ(read_data[7], 'A');
    
    read = fifo_get(&fifo, read_data, 8);
    EXPECT_EQ(read, 8);
    EXPECT_EQ(read_data[0], 'B');
    EXPECT_EQ(read_data[7], 'B');
}

TEST_F(FifoTest, PartialPut) {
    fifo_init(&fifo, buffer, 16);
    
    char write_data[10];
    memset(write_data, 'A', 10);
    fifo_put(&fifo, write_data, 10);
    
    memset(write_data, 'B', 10);
    unsigned int written = fifo_put(&fifo, write_data, 10);
    EXPECT_EQ(written, 6);
}

TEST_F(FifoTest, PartialGet) {
    fifo_init(&fifo, buffer, 16);
    
    char write_data[10];
    memset(write_data, 'A', 10);
    fifo_put(&fifo, write_data, 10);
    
    char read_data[10];
    unsigned int read = fifo_get(&fifo, read_data, 10);
    EXPECT_EQ(read, 10);
}

TEST_F(FifoTest, PutAndGetSequence) {
    fifo_init(&fifo, buffer, 256);
    
    for (int i = 0; i < 10; i++) {
        char data = '0' + i;
        fifo_put(&fifo, &data, 1);
    }
    
    for (int i = 0; i < 10; i++) {
        char data = 0;
        fifo_get(&fifo, &data, 1);
        EXPECT_EQ(data, '0' + i);
    }
}

TEST_F(FifoTest, LargeDataTransfer) {
    fifo_init(&fifo, buffer, 256);
    
    char write_data[200];
    for (int i = 0; i < 200; i++) {
        write_data[i] = i % 256;
    }
    
    unsigned int written = fifo_put(&fifo, write_data, 200);
    EXPECT_EQ(written, 200);
    
    char read_data[200];
    unsigned int read = fifo_get(&fifo, read_data, 200);
    EXPECT_EQ(read, 200);
    
    for (int i = 0; i < 200; i++) {
        EXPECT_EQ(read_data[i], write_data[i]);
    }
}

TEST_F(FifoTest, InterleavedPutAndGet) {
    fifo_init(&fifo, buffer, 256);
    
    char write_data1[50];
    memset(write_data1, 'A', 50);
    fifo_put(&fifo, write_data1, 50);
    
    char read_data[30];
    fifo_get(&fifo, read_data, 30);
    
    char write_data2[50];
    memset(write_data2, 'B', 50);
    fifo_put(&fifo, write_data2, 50);
    
    memset(read_data, 0, 50);
    fifo_get(&fifo, read_data, 50);
    EXPECT_EQ(read_data[0], 'A');
    EXPECT_EQ(read_data[19], 'A');
    EXPECT_EQ(read_data[20], 'B');
    EXPECT_EQ(read_data[49], 'B');
}

TEST_F(FifoTest, ZeroLengthPut) {
    fifo_init(&fifo, buffer, 256);
    
    unsigned int written = fifo_put(&fifo, NULL, 0);
    EXPECT_EQ(written, 0);
}

TEST_F(FifoTest, ZeroLengthGet) {
    fifo_init(&fifo, buffer, 256);
    
    unsigned int read = fifo_get(&fifo, NULL, 0);
    EXPECT_EQ(read, 0);
}

TEST_F(FifoTest, FifoStatePreservation) {
    fifo_init(&fifo, buffer, 256);
    
    char data = 'X';
    fifo_put(&fifo, &data, 1);
    
    EXPECT_EQ(fifo.in, 1);
    EXPECT_EQ(fifo.out, 0);
    
    fifo_get(&fifo, &data, 1);
    
    EXPECT_EQ(fifo.in, 1);
    EXPECT_EQ(fifo.out, 1);
}

struct ThreadTestContext {
    struct _fifo_t* fifo;
    std::atomic<bool> producer_done{false};
    std::atomic<bool> consumer_done{false};
    std::vector<char> produced_data;
    std::vector<char> consumed_data;
    int produce_count;
    int consume_count;
};

void* producer_thread(void* arg) {
    ThreadTestContext* ctx = static_cast<ThreadTestContext*>(arg);
    char data_array[2];
    int i = 0;

    for (i = 0; i < ctx->produce_count; i++) 
    {
        char data = 'A' + (i % 52);
        ctx->produced_data.push_back(data);
        if (i & 0x01)
        {
            data_array[1] = data;
            fifo_put(ctx->fifo, data_array, 2);
        }
        else 
        {
            data_array[0] = data;
        }
    }

    if (i & 0x01) 
    {
        fifo_put(ctx->fifo, data_array, 1);
    }
    
    ctx->producer_done = true;
    
    return nullptr;
}

void* consumer_thread(void* arg) {
    ThreadTestContext* ctx = static_cast<ThreadTestContext*>(arg);

    while (true) {
        char data_array[1024] = {0};
        unsigned int read = fifo_get(ctx->fifo, data_array, sizeof(data_array)/sizeof(data_array[0]));

        if (read > 0) {
            for (unsigned int i = 0; i < read; i++) {
                ctx->consumed_data.push_back(data_array[i]);
            }
        } else if (ctx->producer_done) {
            break;
        }
    }

    ctx->consumer_done = true;
    return nullptr;
}

TEST_F(FifoTest, ProducerConsumerMultiThreaded) {
    ThreadTestContext ctx;
    ctx.fifo = &fifo;
    ctx.produce_count = 0x1111111;
    ctx.consume_count = 0x1111111;
    
    fifo_init(&fifo, buffer, 0x800000);
    
    pthread_t producer, consumer;
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    pthread_create(&producer, &attr, producer_thread, &ctx);
    pthread_create(&consumer, &attr, consumer_thread, &ctx);
    
    pthread_attr_destroy(&attr);
    
    while (!ctx.producer_done || !ctx.consumer_done) {
        sched_yield();
    }
    
    EXPECT_EQ(ctx.produced_data.size(), ctx.consumed_data.size());
    
    for (size_t i = 0; i < ctx.produced_data.size(); i++) {
        ASSERT_EQ(ctx.produced_data[i], ctx.consumed_data[i]) << "Index " << i << " mismatch";
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
