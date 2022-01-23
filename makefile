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

# excelWriter: main.cpp ExcelWriter.cpp InputParser.cpp types.h
# 	g++ \
# 	-O2 \
# 	-L lib \
# 	-I include \
# 	-std=c++20 \
# 	-o excelWriter main.cpp lib/libxlsxwriter.a \
#  	-lz \
# 	-static-libstdc++ \
# 	-Wall -Wextra 

excelWriter: main.cpp ExcelWriter.cpp InputParser.cpp types.h
	g++ \
	-o excelWriter main.cpp lib/libxlsxwriter.a \
	-std=c++20 \
	-O2 \
	-L lib \
	-I include \
 	-lz \
	-Wall -Wextra 