#!/usr/bin/env sh

strace -e '!rt_sigaction,rt_sigprocmask,fstat,openat,access,newfstatat,prlimit64,getrandom,close,read,pread64,rseq,arch_prctl,set_tid_address,set_robust_list,brk,mmap,munmap,madvise,mprotect' $@
