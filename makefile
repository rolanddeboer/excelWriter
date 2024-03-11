excelWriter: main.cpp ExcelWriter.cpp InputParser.cpp types.h
	g++ \
	-o excelWriter main.cpp \
	-std=c++20 \
	-O2 \
	-L lib \
	-I include \
	-lxlsxwriter \
	-lz \
	-Wall -Wextra 