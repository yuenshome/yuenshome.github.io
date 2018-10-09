#include <stdlib.h>
#include <assert.h>
int main() {
	int *p = (int*)malloc(sizeof(int));
	assert(p!=NULL); //assert(p);
	if (p == NULL) exit(-1);
	if (p)	free(p);
	return 0;
}