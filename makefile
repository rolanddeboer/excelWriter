# excelWriter: main.cpp ExcelWriter.cpp InputParser.cpp types.h
# 	g++ \
# 	-O2 \
# 	-L lib \
# 	-I include \
# 	-o excelWriter main.cpp \
# 	-l lib/libxlsxwriter.a \
# 	-lz \
# 	-static-libstdc++ \
# 	-Wall -Wextra

test: main.cpp
	g++ \
	-std=c++0x \
	-o test main.cpp