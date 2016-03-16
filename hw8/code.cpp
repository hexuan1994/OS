#include <iostream>
#include <stdio.h>

using namespace std;

const int baseAddr = 0xd80;

int mem[4096];
int disk[4096];

void load(){
	//load mem
	FILE *fm = fopen("mem.txt","r");
	for(int i = 0;i < 0x80;i++){
		int temp;
		fscanf(fm, "page %x:",&temp);
		for(int j = 0;j < 0x20;j++){
			fscanf(fm, " %x",&mem[i*0x20+j]);
		}
		fscanf(fm, "\n");
	}
	//load dist
	FILE *fd = fopen("disk.txt","r");
	for(int i = 0;i < 0x80;i++){
		int temp;
		fscanf(fd, "disk %x:",&temp);
		for(int j = 0;j < 0x20;j++){
			fscanf(fd, " %x",&disk[i*0x20+j]);
		}
		fscanf(fd, "\n");
	}
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
	int addr = (pte_pfn << 5) | (logicAddr & 0x1f);
	if( pte_valid == 0){
		if(pte_pfn == 0x7f){
			printf("    --> Fault (page directory entry not valid)\n");
		}else{
			printf("    --> To Disk Sector Address 0x%x --> Value: 0x%x\n",addr,disk[addr]);
		}
		return;
	}else{
		printf("      --> To Physical Address 0x%x --> Value: 0x%x\n", addr, mem[addr]);
	}
}

int main(){
	load();
	map(0x1c13);
	// cout << mem[0x7+0x80]<<endl;
	// for(int k = 0;k < 5;k++){
	// 	for(int i = 0;i < 0x20;i++){
	// 		cout << mem[k*0x20 + i]<< ' ';
	// 	}
	// 	cout << endl;
	// }
	return 0;
}