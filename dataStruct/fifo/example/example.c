#include <stdio.h>
#include <string.h>
#include "../fifo.h"

#define BUFFER_SIZE 256

int main(void) {
    struct _fifo_t fifo;
    unsigned char buffer[BUFFER_SIZE];
    
    // Initialize FIFO
    if (fifo_init(&fifo, buffer, BUFFER_SIZE) != 0) {
        printf("FIFO initialization failed\n");
        return -1;
    }
    
    printf("FIFO initialized successfully (size: %u)\n", fifo.size);
    
    // Write data to FIFO
    char write_data1[] = "Hello, World!";
    unsigned int written = fifo_put(&fifo, write_data1, strlen(write_data1));
    printf("Written %u bytes: %s\n", written, write_data1);
    
    char write_data2[] = "This is a FIFO example.";
    written = fifo_put(&fifo, write_data2, strlen(write_data2));
    printf("Written %u bytes: %s\n", written, write_data2);
    
    // Read data from FIFO
    char read_data[100];
    unsigned int read = fifo_get(&fifo, read_data, sizeof(read_data));
    read_data[read] = '\0';
    printf("Read %u bytes: %s\n", read, read_data);
    
    // Read remaining data
    read = fifo_get(&fifo, read_data, sizeof(read_data));
    read_data[read] = '\0';
    printf("Read %u bytes: %s\n", read, read_data);
    
    // Test wrap-around behavior
    printf("\nTesting wrap-around:\n");
    
    // Fill the buffer
    char fill_data[BUFFER_SIZE];
    memset(fill_data, 'A', BUFFER_SIZE);
    written = fifo_put(&fifo, fill_data, BUFFER_SIZE);
    printf("Filled buffer with %u bytes\n", written);
    
    // Read half
    read = fifo_get(&fifo, read_data, BUFFER_SIZE / 2);
    printf("Read %u bytes\n", read);
    
    // Write more data (should wrap around)
    char wrap_data[] = "WRAP";
    written = fifo_put(&fifo, wrap_data, strlen(wrap_data));
    printf("Written %u bytes after wrap: %s\n", written, wrap_data);
    
    // Read all remaining data
    while ((read = fifo_get(&fifo, read_data, sizeof(read_data))) > 0) {
        printf("Read %u bytes\n", read);
    }
    
    printf("\nFIFO example completed successfully\n");
    return 0;
}
