#ifndef UECSRXDUMP_CCM_H
#define UECSRXDUMP_CCM_H

#include <stddef.h>

void ccm_strip(char *out, size_t outsz, const char *in);
int  ccm_to_csv(char *out, size_t outsz, const char *in);

#endif
