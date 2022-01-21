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

test: main.cpp ExcelWriter.cpp InputParser.cpp types.h
	g++ \
	-O2 \
	-L lib \
	-I include \
	-std=c++20 \
	-o test main.cpp \
# 	-l xlsxwriter.a \
 	-lz \
	-static-libstdc++ \
	-Wall -Wextra 