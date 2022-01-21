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

excelWriter: main.cpp ExcelWriter.cpp InputParser.cpp types.h
	g++ \
	-O2 \
	-L lib \
	-I include \
	-I /usr/local/include \
	-L /usr/local/lib \
	-std=c++20 \
	-o excelWriter main.cpp \
 	-lxlsxwriter \
 	-lz \
	-static-libstdc++ \
	-Wall -Wextra 