excelWriter: main.cpp ExcelWriter.cpp InputParser.cpp types.h
	/usr/local/opt/gcc/bin/g++-11 \
	-std=c++20 \
	-O2 \
	-L lib \
	-I include \
	-o excelWriter main.cpp \
	-l lib/libxlsxwriter.a \
	-lz \
	-static-libstdc++ \
	-Wall -Wextra