#ifndef COMMON_H_
#define COMMON_H_

// Nicer way to call bpf_trace_printk()
#define bpf_custom_printk(fmt, ...)						\
		({							\
			char ____fmt[] = fmt;				\
			bpf_trace_printk(____fmt, sizeof(____fmt),	\
				     ##__VA_ARGS__);			\
		})

#endif /* COMMON_H_ */