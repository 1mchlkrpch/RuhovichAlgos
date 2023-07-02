TARGET := BFPRT_algorithm.cpp
EXECUTABLE := algo

all: $(EXECUTABLE)
	$(EXECUTABLE)

$(EXECUTABLE): $(TARGET)
	g++ -g -Wall -Wextra -Wpedantic $< -o $@

clean:
	@rm $(TARGET)
