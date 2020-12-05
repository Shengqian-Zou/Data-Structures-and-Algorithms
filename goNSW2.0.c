#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "PQueue.h"

#define VERY_HIGH_VALUE 999999

typedef struct stops {
    char **stops_name; //定义一个数组储存名字
    int *mark_number;
    int stops_num //一共有多少车站
    int *time;  //储存时间
} Stops;

void showPath(int v, int pred[]) {
    if (pred[v] == -1) {
        printf("%d", v);
        } else {
        showPath(pred[v], pred);
        printf("-%d", v);
        }
    }

void dijkstraSSSP(Graph g, Vertex source) {
    int dist[MAX_NODES];
    int pred[MAX_NODES];
    bool vSet[MAX_NODES];  // vSet[v] = true <=> v has not been processed
    int s;

    PQueueInit();
    int nV = numOfVertices(g);
    for (s = 0; s < nV; s++) {
        joinPQueue(s);
        dist[s] = VERY_HIGH_VALUE;
        pred[s] = -1;
        vSet[s] = true;
    }
    dist[source] = 0;

    while (!PQueueIsEmpty(g)) {
        int i, a;
        a = leavePQueue(dist);
        vSet[a] = false;
        for (i = 0; i < nV; i++) {
            if (vSet[i] == true) {
                if (adjacent(g, a, i) + dist[a] < dist[i] && adjacent(g, a, i) > 0) {
                    dist[i] = adjacent(g, a, i) + dist[a];
                    pred[i] = a;
                }
            }
        }
    }

//    for (s = 0; s < nV; s++) {
//        printf("%d: ", s);
//        if (dist[s] < VERY_HIGH_VALUE) {
//            printf("distance = %d, shortest path: ", dist[s]);
//            showPath(s, pred);
//            putchar('\n');
//            } else {
//                printf("no path\n");
//            }
//        }
    int b, d, t,o;
    int c[nV];

    for (b=0;b<nV;b++) {
        for(t=0;t<nV;t++){
            c[t] = -1;
        }
        int prednum = 0;
        if (dist[b] >= 0 && dist[b] < VERY_HIGH_VALUE) {
            d = 0;
            o = b;
            c[d] = b;
            printf("%d:distance = %d, shortest path: ", b, dist[b]);
            while (pred[b] >= 0) {
                b = pred[b];
                d = d + 1;
                c[d] = b;
            }
            for (t = 0; t < nV; t++) {
                if (c[t] >= 0 && c[t] < VERY_HIGH_VALUE) {
                    prednum++;
                }
            }
            prednum--;
            for (t = prednum; t > 0; t--) {
                if (c[t] >= 0 && c[t] < VERY_HIGH_VALUE) {
                    printf("%d-", c[t]);
                }
            }
            printf("%d\n", c[0]);
            b = o;

        } else { printf("%d: no path\n", b); }

    }
}

void reverseEdge(Edge *e) {
   Vertex temp = e->v;
   e->v = e->w;
   e->w = temp;
}

int main(){
//分配一个数组储存刚开始的站台名字
    int stop_number;
    char stop_name[32];  //规定最大不超过32个
    printf("Enter the total number of stops on the network: ");
    if(scanf("%d", &stop_number)==1) {
        char **stops_str = malloc(stop_number * sizeof(char *));
        assert(stops_str != NULL);
        for (int i = 0; i < stop_number; i++) {
            if (scanf("%s", stop_name) == 1) {
                stops_str[i] = malloc((strlen(stop_name) + 1) * sizeof(char));  //+1是为了给\0一个位置
                strcpy(stops_str[i], stop_name);
            }

        }
        for (int i = 0; i < stop_number; i++) {
            printf("%s\n", stops_str[i]);
        }
    }

    printf("Enter the number of schedules: ");
    int schedules_number;
    int marks = 1;
    if(scanf("%d", &schedules_number)==1) { //统计有多少条路线
        Stops *stops_line = malloc(schedules_number * sizeof(Stops)); //分配空间
        assert(stops_line != NULL);
        for (int i = 0; i < schedules_number; i++) {
            int times;
            int stops_number;
            char stops_name[32];
            printf("Enter the number of stops: ");
            if (scanf("%d", &stops_number) == 1) {
            	stops_line[i].stops_num = stops_number; 
                char **input = malloc((stops_number*2) * sizeof(char *));//分配一个起始数组把时间和站名都存进去
                assert(input!=NULL);
                stops_line[i].stops_name = malloc(stops_number * sizeof(char *)); //给站名分配空间
                assert(stops_line[i].stops_name != NULL);
                stops_line[i].time = malloc(stops_number * sizeof(int));
                assert(stops_line[i].time != NULL);
                stops_line[i].mark_number = malloc(stops_number * sizeof(int));
                assert(stops_line[i].mark_number != NULL);
                for (int a = 0; a < stops_number*2; a++) {
                    if (scanf("%s", stops_name) == 1) {  //保存车站名字
                        input[a] = malloc((strlen(stops_name) + 1) * sizeof(char));  //+1是为了给\0一个位置
                        strcpy(input[a], stops_name);
                    }
                }
                /*
                for (int i = 0; i < stops_number*2; i++) {
                    printf("%s", input[i]);
                }*/
                
                for(int c = 0; c < stops_number; c++){
                    stops_line[i].stops_name[c] = malloc((strlen(stops_name) + 1) * sizeof(char));
                    assert(stops_line[i].stops_name[c] != NULL);
                    strcpy(stops_line[i].stops_name[c], input[c*2+1]);
                    //stops_line[i].time[c] = malloc((strlen(stops_name) + 1) * sizeof(char));
                    //assert(stops_line[i].time[c] != NULL);
                    //strcpy(stops_line[i].time[c], input[c*2]);
                    stops_line[i].time[c] = atoi(input[c*2]);
                    stops_line[i].mark_number[c] = marks;
                    marks++;
                }
               
            }
        }
        for (int a = 0; a < 2; a++) {
            for (int i = 0; i < 3; i++) {
                printf("%s %d %d", stops_line[a].stops_name[i], stops_line[a].mark_number[i], stops_line[a].time[i]);
            }
        }
        //构图
        Edge e;
        for (int a = 0; a < schedules_number; a++){
        	for (int b = 0; b < stops_line[a].stops_num; b++){
        		Graph goNSW = newGraph(stops_line[a].mark_number[b]);//用他们的marknumber作为节点，从1开始
                if(stops_line[a].mark_number[b] < stops_number+1){
                	int weight = ((stops_line[a].time[b+1]/100)*60 + stops_line[a].time[b+1]%100) - ((stops_line[a].time[b]/100)*60 + stops_line[a].time[b]%100);
                	e.v = stops_line[a].mark_number[b];
                	e.w = stops_line[a].mark_number[b+1];
                	e.weight = weight;
                	insertEdge(goNSW, e);
                	reverseEdge(&e);
                	insertEdge(g, e);

                }
                for(int c = 0; c < schedules_number; c++){
                	for(int d = 0; d <stops_line[c].stops_num; b++){
                		if(strcmp(stops_line[a].stops_name[b], stops_line[c].stops_name[d]) == 0){
                			e.v = stops_line[a].stops_name[b];
                			e.w = stops_line[c].stops_name[d];
                			e.weight = ((stops_line[c].time[d]/100)*60 + stops_line[c].time[d]%100) - ((stops_line[a].time[b]/100)*60 + stops_line[a].time[b]%100);
                			insertEdge(goNSW, e);
                			reverseEdge(&e);
                			insertEdge(g, e);
                		}
                	}
                }
        	}

        }
        printf("From: ");
        char inputfrom[32];
        char inputto[32];
        int sourse, endpoint, strtime;
        char end[5] = "done";
        scanf("%s", inputfrom);
        while(strcmp(inputfrom, end) != 0){
        	sourse = 0;
        	endpoint = 99;
        	printf("To: ");
        	scanf("%s", inputto);
        	printf("Depart at: ");
        	scanf("%d", &strtime);
        	for(int a = 0; a < schedules_number; a++){ //找到开始的车站建立边
        		for(int b = 0; b < stops_line[a].stops_num; b++){
        			if(strcmp(stops_line[a].stops_name[b], inputfrom) == 0){
        				e.v = sourse;
        				e.w = stops_line[a].mark_number[b];
        				e.weight = ((stops_line[a].time[b]/100)*60 + stops_line[a].time[b]%100) - ((strtime/100)*60 +strtime%100);
        				insertEdge(goNSW, e);
                		reverseEdge(&e);
                		insertEdge(g, e);
        			}
        		}
        	}
        	//找到到目标点的最短路径
        	//这里记得算出路线
        	//free初始点和结束点和边
        	printf("From: ");
        	scanf("%s", inputfrom);
        }
    }
    return 0;
}