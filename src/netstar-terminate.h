#ifndef _NETSTAR_TERMINATE_H
#define _NETSTAR_TERMINATE_H

#ifdef __cplusplus
extern "C" {
#endif

#define NETSTAR_EXIT_SUCCESS 0x00000000
#define NETSTAR_EXIT_FAILURE 0x00000001


extern void netstar_terminate(void);
extern void netstar_exit(int);

#ifdef __cplusplus
}
#endif

#endif
