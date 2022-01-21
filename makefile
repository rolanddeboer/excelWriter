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

excelWriter: main.cpp ExcelWriter.cpp InputParser.cpp types.h lib/libxlsxwriter.a
	g++ \
	-O2 \
	-L lib \
	-I include \
	-std=c++20 \
	-o excelWriter main.cpp \
 	-lz \
	-static-libstdc++ \
	-Wall -Wextra 