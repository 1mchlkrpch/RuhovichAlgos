TARGET := groupAnagrams.cpp
EXECUTABLE := algo

all: $(EXECUTABLE)
	@./$(EXECUTABLE) < input

$(EXECUTABLE): $(TARGET)
	g++ -g -Wno-unused-variable -Waggressive-loop-optimizations -Wall -Wextra -Wpedantic $< -o $@
# 
clean:
	@rm $(EXECUTABLE)
