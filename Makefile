CC = g++
# path assignment
WORK_DIR = ./
INC_PATH = $(join $(WORK_DIR), inc)
SRC_PATH = $(join $(WORK_DIR), src)
OBJ_PATH = $(join $(WORK_DIR), obj)
BIN_PATH = $(join $(WORK_DIR), bin)

# src/*.cpp
SRC_FILE_WITH_DIR = $(wildcard $(join $(SRC_PATH), /*.cpp))

# *.cpp
SRC_FILE = $(notdir $(SRC_FILE_WITH_DIR))
$(warning  $(SRC_FILE))

# *.o
OBJ_FILE = $(SRC_FILE:%.cpp=%.o)
$(warning  $(OBJ_FILE))

# obj/*.o
OBJ_FILE_WITH_DIR = $(patsubst %.o, $(OBJ_PATH)/%.o, $(OBJ_FILE))

TARGET = $(join $(BIN_PATH), /start_game)

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.cpp
	$(CC) -I$(INC_PATH) -c $< -o $@

$(TARGET) : $(OBJ_FILE_WITH_DIR)
	$(CC) -o $@ $^

.PHONY: clean
clean:
	@rm $(OBJ_FILE_WITH_DIR) # @表示不在终端打印执行的命令
	@rm $(TARGET)





#pacmanaimain:main.o MySocketClient.o AmazingPacManAi.o
#	g++ main.o MySocketClient.o AmanzingPacManAi.o -o maincc
#main.o:main.cpp MyDefine.h
#	g++ -c main.cpp
#AmazingPacManAi.o:AmazingPacManAi.cpp
#	g++ -c AmazingPacManAi.cpp
#MySocketClient.o:MySocketClient.cpp
#	g++ -c MySocketClient.cpp
#clean :
#	rm *.o
