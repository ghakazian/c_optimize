C_COMPILER=cc
OUTPUT_PATH=build

$(shell mkdir -p ${OUTPUT_PATH})

default: test_j

test_j: test_j.c
	${C_COMPILER} test_j.c -o ${OUTPUT_PATH}/test_j

clean:
	rm -rf ${OUTPUT_PATH}

time: test_j
	time ${OUTPUT_PATH}/test_j


.PHONY:	default
.PHONY: clean
.PHONY: time
