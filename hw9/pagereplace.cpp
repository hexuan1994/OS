#include <iostream>
#include <stdio.h>
#include <memory.h>

const int T = 2;
const int PAGE_SIZE = 10;
int page[PAGE_SIZE];
int currentTime, lastTime;

void init(){
    memset(page,-1,sizeof(page));
    currentTime = 0;
    lastTime = -1;
    //you can initialize it 
    /*
    */
}

int main(int argc,char* argv[]){
    init();
    while(true){
        int visitPageNo;
        printf("Please input the page number: \n");
        scanf("%d",&visitPageNo);
        if(visitPageNo <= 0 || visitPageNo >= PAGE_SIZE)
            printf("Error, please input the page number.\n");
        if(page[visitPageNo] != -1) {
            printf("Page[%d] hit!\n",visitPageNo);
            page[visitPageNo] = currentTime;
        } else {
            printf("Page[%d] miss!\n",visitPageNo);
            if(currentTime - lastTime <= T){
                page[visitPageNo] = currentTime;
                lastTime = currentTime;
                printf("Page[%d] has been added.\n",visitPageNo);
            } else {
                for(int i = 0;i < PAGE_SIZE;i++){
                    if(page[i] != -1 && (currentTime - page[i] > T)){
                        page[i] = -1;
                        printf("Page[%d] has been replaced.\n",i);
                    }
                }
                page[visitPageNo] = currentTime;
                lastTime = currentTime;
                printf("Page[%d] has been added.\n",visitPageNo);
            }
        }
        //print page[]
        printf("Page: \n");
        for(int i = 0;i < PAGE_SIZE;i++){
            printf("page[%d] = %4d ",i,page[i]);
            if((i + 1) % 5 == 0) printf("\n");
        }
        printf("\n");
        printf("currentTime = %d lastTime = %d\n",currentTime,lastTime);
        currentTime++;
    }

}