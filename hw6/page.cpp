#include <iostream>
#include <algorithm>
#include <stdio.h>

using namespace std;

int mem[0x80*0x20];
int baseAddr = 0x0;


int loadMem(char* filename){
	FILE* fp = fopen(filename,"r");
	if(!fp){
		printf("Fail to open the file: %s\n",filename);
		return -1;
	}
	int i = 0,j = 0;
	while(true){

		if(fscanf(fp,"page %x:",&i) == EOF)
			break;
		int temp = 0;
		for(j = 0;j < 0x20;j++){
			fscanf(fp,"%x",&temp);
			mem[i*0x20 + j] = temp;
			printf("%x ",temp);
		}
		fscanf(fp,"\n");
		printf("\n");
	}
	return 1;
}

void map(int logicAddr){
	int pde_index = logicAddr >> 10 & 0x1f;
	int pde_contents = mem[baseAddr + pde_index];
	int pde_valid = pde_contents >> 7 & 0x1;
	int pde_pfn = pde_contents & 0x7f;
	printf("Virtual Address %04x:\n",logicAddr);
	printf("  --> pde index:0x%02x  pde contents:(valid %d, pfn 0x%02x)\n", pde_index, pde_valid, pde_pfn);
	if( pde_valid == 0){
		printf("    --> Fault (page directory entry not valid)\n");
		return;
	}

	int pte_index = logicAddr >> 5 & 0x1f;
	int pte_contents = mem[pde_pfn << 5 | pte_index];
	int pte_valid = pte_contents >> 7 & 0x1;
	int pte_pfn = pte_contents & 0x7f;
	printf("    --> pte index:0x%02x  pte contents:(valid %d, pfn 0x%02x)\n", pte_index, pte_valid, pte_pfn);
	if( pte_valid == 0){
		printf("    --> Fault (page directory entry not valid)\n");
		return;
	}
	int realAddr = (pte_pfn<<5) + 0x1f;
	printf("      --> Translates to Physical Address 0x%x --> Value: 0x%x\n", realAddr, mem[realAddr]);

}


int main(int argc,char* argv[]){
	char filename[20];
	sscanf(argv[1],"%s",&filename);
	sscanf(argv[2],"%x",&baseAddr);
	if(loadMem(filename) == -1)
		return 0 ;
	for(int k = 0;k < 64;k++)
		printf("%x ",mem[k]);
	while(true){
		printf("LogicAddr: \n");
		int addr = 0;
		scanf("%x",&addr);
		if(addr == -1) break;
		map(addr);
	}
	return 0;
}
