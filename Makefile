SRCDIR = ./src

INCLUDEDIR = ./include

RESOURCEDIR = ./resource

BUILDDIR = ./build

target_file = ${BUILDDIR}/main.exec

cpps = ${shell find ${SRCDIR} -name *.cpp -or -name *.c}
objs = ${patsubst ${SRCDIR}/%,${BUILDDIR}/%.o,${cpps}}
hpps = ${shell find ${INCLUDEDIR} -name *.h -or -name *.hpp}

CXXFLAGS = -g -Wall -std=c++11

IFLAGS = -I${INCLUDEDIR}

all : ${target_file}

${target_file} : ${objs}
	@${CXX} ${objs} -o ${target_file}
	@chmod u+x ${target_file}

${objs} : ${BUILDDIR}/%.o : ${SRCDIR}/% ${hpps}
	@mkdir -p ${BUILDDIR}
	@${CXX} ${CXXFLAGS} ${IFLAGS} -c $< -o $@

test:
	@echo ${objs}

clearn:
	rm -rf build

.PHONY: all test clearn