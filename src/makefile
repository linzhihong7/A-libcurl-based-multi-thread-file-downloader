CC = g++
CFLAGS = -Wall -std=c++11

SRC = main.cpp httpDownLoader.cpp util.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = download_tool

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -lcurl -lcrypto

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean
clean:
	-rm -f $(OBJ) $(TARGET)
