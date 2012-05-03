#
# Make file for aisp
#
SRC=src/*.cc
INCLUDE=./include

all:
	g++ ${SRC} -I${INCLUDE} -o aisp

clean:
	rm -f src/*.o
	rm -f aisp
