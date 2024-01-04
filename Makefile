#resource file path
SRCDIR = ./src
#header file path
INCLUDEDIR = ./include

RESOURCEDIR = ./resource
#build path
BUILDDIR = ./build
#target file path
target_file = ${BUILDDIR}/main.exec

cpps = ${shell find ${SRCDIR} -name *.cpp -or -name *.c}
objs = ${patsubst ${SRCDIR}/%,${BUILDDIR}/%.o,${cpps}}
hpps = ${shell find ${INCLUDEDIR} -name *.h -or -name *.hpp}

CXXFLAGS = -g -Wall -std=c++11

IFLAGS = -I${INCLUDEDIR}

all : ${target_file}

#link target file
${target_file} : ${objs}
	@${CXX} ${objs} -o ${target_file}
	@chmod u+x ${target_file}

#compile resource c/cpp files
${objs} : ${BUILDDIR}/%.o : ${SRCDIR}/% ${hpps}
	@mkdir -p ${BUILDDIR}
	@${CXX} ${CXXFLAGS} ${IFLAGS} -c $< -o $@

test:
	@echo ${objs}

#delete build files
clearn:
	rm -rf build

.PHONY: all test clearn