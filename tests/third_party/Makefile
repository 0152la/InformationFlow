all: ${out}

${out}:
	clang -g -emit-llvm -S ${flags} $(@:%.ll=%.c)

clean:
	rm ${out}

cleanall:
	find . -type f -name "*.ll" -print -delete

.PHONY: all clean cleanall
