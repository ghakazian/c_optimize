C_COMPILER=cc
OUTPUT_PATH=build

$(shell mkdir -p ${OUTPUT_PATH})

default: test_j test_j_optimized

test_j: test_j.c
	${C_COMPILER} test_j.c -o ${OUTPUT_PATH}/test_j

test_j_optimized: test_j_optimized.c
	${C_COMPILER} test_j_optimized.c -o ${OUTPUT_PATH}/test_j_optimized
clean:
	rm -rf ${OUTPUT_PATH}

time: test_j test_j_optimized
	echo "Timing original code..."
	time ${OUTPUT_PATH}/test_j
	echo "Timing optimized code..."
	time ${OUTPUT_PATH}/test_j_optimized

.PHONY:	default
.PHONY: clean
.PHONY: time
