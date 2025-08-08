CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = ytdl
SOURCE = youtubeDownload.c

# Default target
all: $(TARGET)

# Compile the file analyzer
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Clean up compiled files
clean:
	rm -f $(TARGET)

# Install to /usr/local/bin (optional)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Create a test executable to analyze
test_executable:
	$(CC) -o test_program -fstack-protector-all -fPIE -pie test_program.c

# Help target
help:
	@echo "Available targets:"
	@echo "  all            - Compile the ytdl"
	@echo "  clean          - Remove compiled files"
	@echo "  install        - Install to /usr/local/bin"
	@echo "  test_MachO     - Create a test program with security features"
	@echo "  help           - Show this help message"

.PHONY: all clean install ytdl help