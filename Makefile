# to compile run:
# mingw32-make
# .\number_game.exe

CXX = g++
CXXFLAGS = -Wall -Wextra -g3
TARGET = number_game.exe
SRCS = $(wildcard *.cpp)

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	@if exist $(TARGET) del /f /q $(TARGET)