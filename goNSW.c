#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "PQueue.h"

#define VERY_HIGH_VALUE 999999

typedef struct stops {
    char **stops_name; //Define an array for storing station names
    int *mark_number; //Assign a unique identification code to each station
    int stops_num; //Store how many stations are in each line
    char **time;  //Store time for each stations
} Stops;

void showPath(int v, int pred[]) {
    if (pred[v] == -1) {
        printf("%d", v);
        } else {
        showPath(pred[v], pred);
        printf("-%d", v);
        }
    }

int minimumW(Graph g, Vertex source, Vertex dest) { //Calculate the shortest path and return time (weight)
    int dist[MAX_NODES];
    bool vSet[MAX_NODES];  
    int s;

    PQueueInit();
    int nV = numOfVertices(g);
    for (s = 0; s < nV; s++) {
        joinPQueue(s);
        dist[s] = VERY_HIGH_VALUE;
        vSet[s] = true;
    }
    dist[source] = 0;

    while (!PQueueIsEmpty(g)) {
        int a;
        a = leavePQueue(dist);
        vSet[a] = false;
        for (int i = 0; i < nV; i++) {
            if (vSet[i] == true) {
                if (adjacent(g, a, i) + dist[a] < dist[i] && adjacent(g, a, i) > 0) {
                    dist[i] = adjacent(g, a, i) + dist[a];
                }
            }
        }
    }

    int weight;
    weight = dist[dest];
    return weight;
}

int *shortline(Graph g, Vertex source, Vertex dest) { //Calculate the shortest path and the array of paths (pointer variables)
    int dist[MAX_NODES];
    int pred[MAX_NODES];
    bool vSet[MAX_NODES]; 
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
        int a;
        a = leavePQueue(dist);
        vSet[a] = false;
        for (int i = 0; i < nV; i++) {
            if (vSet[i] == true) {
                if (adjacent(g, a, i) + dist[a] < dist[i] && adjacent(g, a, i) > 0) {
                    dist[i] = adjacent(g, a, i) + dist[a];
                    pred[i] = a;
                }
            }
        }
    }
    int b, d, t,o;
    int c[nV];
    int *output = malloc(nV* sizeof(int));

    for (b=0;b<nV;b++) {
        for(t=0;t<nV;t++){
            c[t] = -1;
            output[t] = -1;
        }
        int prednum = 0;
        if (dist[b] >= 0 && dist[b] < VERY_HIGH_VALUE) {
            d = 0;
            o = b;
            c[d] = b;
            //printf("%d:distance = %d, shortest path: ", b, dist[b]);
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
            s = 0;
            for (t = prednum; t > 0; t--) {
                if (c[t] >= 0 && c[t] < VERY_HIGH_VALUE) {
                	output[s] = c[t];
                	s++;
                    //printf("%d-", c[t]);

                }
            }
            c[s] = c[0];
            //printf("%d\n", c[0]);
            b = o;

        }
    }
    return output;
}

int main(){
    int stop_number;
    char stop_name[32];  //The maximum length of the name does not exceed 32
    printf("Enter the total number of stops on the network: ");
    if(scanf("%d", &stop_number)==1) {
        char **stops_str = malloc(stop_number * sizeof(char *));
        assert(stops_str != NULL);
        for (int i = 0; i < stop_number; i++) {
            if (scanf("%s", stop_name) == 1) {
                stops_str[i] = malloc((strlen(stop_name) + 1) * sizeof(char));  //+1 is to assign a position to \0
                strcpy(stops_str[i], stop_name);
            }

        }
        for (int i = 0; i < stop_number; i++){
        	free(stops_str[i]);
        	}
        free(stops_str);

    }

    printf("Enter the number of schedules: ");
    int schedules_number;
    int marks = 1;
    if(scanf("%d", &schedules_number)==1) { //Count how many routes there are and make sure to input the integer type
        Stops *stops_line = malloc(schedules_number * sizeof(Stops));
        assert(stops_line != NULL);
        for (int i = 0; i < schedules_number; i++) {
            int stops_number;
            char stops_name[32];
            printf("Enter the number of stops: ");
            if (scanf("%d", &stops_number) == 1) { //make sure to input the integer type
            	stops_line[i].stops_num = stops_number; 
                char **input = malloc((stops_number*2) * sizeof(char *)); //Assign an array to store the station name and time
                assert(input!=NULL);
                stops_line[i].stops_name = malloc(stops_number * sizeof(char *)); 
                assert(stops_line[i].stops_name != NULL);
                stops_line[i].time = malloc(stops_number * sizeof(char *));
                assert(stops_line[i].time != NULL);
                stops_line[i].mark_number = malloc(stops_number * sizeof(int));
                assert(stops_line[i].mark_number != NULL);
                for (int a = 0; a < stops_number*2; a++) {
                    if (scanf("%s", stops_name) == 1) { 
                        input[a] = malloc((strlen(stops_name) + 1) * sizeof(char)); //+1 is to assign a position to \0
                        assert(input[a] != NULL);  
                        strcpy(input[a], stops_name); //copy the stops name to input[a]
                    }
                }
   
                for(int c = 0; c < stops_number; c++){ //copy the stops name and times
                    stops_line[i].stops_name[c] = malloc(32 * sizeof(char));
                    assert(stops_line[i].stops_name[c] != NULL);
                    strcpy(stops_line[i].stops_name[c], input[c*2+1]);           
                    stops_line[i].time[c] = malloc(5 * sizeof(char));
                    assert(stops_line[i].time[c] != NULL);
                    strcpy(stops_line[i].time[c], input[c*2]);
                    //stops_line[i].time[c] = atoi(input[c*2]);
                    stops_line[i].mark_number[c] = marks;
                    marks++;
                }

                for(int f = 0; f < (stops_number*2); f++){  //end to use it, free it
        			free(input[f]);
        			}
        		free(input);
               
            }
        }
              
        Edge e; //Start composing
        int max_node = stops_line[schedules_number-1].mark_number[stops_line[schedules_number-1].stops_num-1];
        Graph goNSW = newGraph(max_node+2); //Use their marknumber as the node, starting from 1, +1 is the starting point of +0
        for (int a = 0; a < schedules_number; a++){
        	for (int b = 0; b < (stops_line[a].stops_num-1); b++){       		
                int weight = (((atoi(stops_line[a].time[b+1])/100)*60 + atoi(stops_line[a].time[b+1])%100) - ((atoi(stops_line[a].time[b])/100)*60 + atoi(stops_line[a].time[b])%100));
                e.v = stops_line[a].mark_number[b];
                e.w = (stops_line[a].mark_number[b]+1);
                e.weight = weight; //Connect the stations in each line separately
                insertEdge(goNSW, e); //Directed graphs only need to insert one edge        
            }
        }
        for (int a = 0; a < (schedules_number-1); a++){ //Connect different lines with the same station name
        	for (int b = 0; b < stops_line[a].stops_num; b++){       		
                for(int c = 1; c < schedules_number; c++){
                	for(int d = 0; d <stops_line[c].stops_num; d++){
                		if(strcmp(stops_line[a].stops_name[b], stops_line[c].stops_name[d]) == 0 && atoi(stops_line[c].time[d]) > atoi(stops_line[a].time[b])){
                			e.v = stops_line[a].mark_number[b];
                			e.w = stops_line[c].mark_number[d];
                			e.weight = (((atoi(stops_line[c].time[d])/100)*60 + atoi(stops_line[c].time[d])%100) - ((atoi(stops_line[a].time[b])/100)*60 + atoi(stops_line[a].time[b])%100));
                			insertEdge(goNSW, e);
                		}
                		else if(strcmp(stops_line[a].stops_name[b], stops_line[c].stops_name[d]) == 0 && atoi(stops_line[c].time[d]) < atoi(stops_line[a].time[b])){
                			e.v = stops_line[c].mark_number[d];
                			e.w = stops_line[a].mark_number[b];
                			e.weight = (((atoi(stops_line[a].time[b])/100)*60 + atoi(stops_line[a].time[b])%100) - ((atoi(stops_line[c].time[d])/100)*60 + atoi(stops_line[c].time[d])%100));
                			insertEdge(goNSW, e);
                		}
                	}
                }
        	}

        }
        printf("\n");
        printf("From: ");
        char inputfrom[32];
        char inputto[32];
        int destination = max_node + 1;
        char strtime[5];
        char end[5] = "done";
        scanf("%s", inputfrom);
        while(strcmp(inputfrom, end) != 0){ //Make sure the input is not "done"
        	printf("To: ");
        	scanf("%s", inputto);
        	printf("Depart at: ");
        	scanf("%s", strtime);
        	printf("\n");
        	int start_number = 0;
        	for(int a = 0; a < schedules_number; a++){ //Calculate the number of starting points.
        		for(int b = 0; b < stops_line[a].stops_num; b++){    			
        			if(strcmp(stops_line[a].stops_name[b], inputfrom) == 0 && atoi(stops_line[a].time[b]) >= atoi(strtime)){
        				start_number += 1;               		
        			}
        			if(strcmp(stops_line[a].stops_name[b], inputto) == 0){ //Connect the end point to a point and use this point as the final point.
        				e.v = stops_line[a].mark_number[b];
        				e.w = destination;
        				e.weight = 1; //A weight of 0 cannot be added to the edge, assuming that it is all 1 (this does not affect the result)
        				insertEdge(goNSW, e); 
        			}
        		}
        	}
        	if(start_number > 0){ //Make sure there is a starting point, otherwise there is no 
        		int each_start[start_number];
        		int each_time[start_number];
        		int each_weight[start_number];
        		for(int i=0; i<start_number;i++){
        			each_start[i] = 0;
        			each_weight[i] = 0;
        		}
        		int addnum = 0; //Create an index starting from 0
        		for(int a = 0; a < schedules_number; a++){
        			for(int b = 0; b < stops_line[a].stops_num; b++){
        				if(strcmp(stops_line[a].stops_name[b], inputfrom) == 0 && atoi(stops_line[a].time[b]) >= atoi(strtime)){
        					each_start[addnum] = stops_line[a].mark_number[b];
        					each_time[addnum] = atoi(stops_line[a].time[b]);
        					addnum += 1;
        				}
        			}
        		}
        		for(int i = 0; i < start_number; i++){ //Calculate the shortest time of the path for each starting point
        			each_weight[i] = minimumW(goNSW, each_start[i], destination);
        		}
        		for(int i=0; i<start_number;i++){ //Add the departure time to the departure time at the station
        			each_weight[i] += (((each_time[i]/100)*60 + each_time[i]%100) - ((atoi(strtime)/100)*60 +atoi(strtime)%100));
        		}
        		
        		int minmub = VERY_HIGH_VALUE;
        		int finall_start = -1;
        		int remember_time = 0;
        		for(int i=0; i<start_number;i++){ //Calculate the shortest time route, if the same time, choose the one with a late departure time
        			if(minmub > each_weight[i]){
        				minmub = each_weight[i];
        				remember_time = each_time[i];
        				finall_start = each_start[i];
        			}
        			if(minmub == each_weight[i]){
        				if(remember_time < each_time[i]){
        					remember_time = each_time[i];
        					finall_start = each_start[i];
        				}
        			}
        		}

        		int *finall_line = shortline(goNSW, finall_start, destination);
        		
        		char tempname[32] = "aaaa";
        		for(int i=0;i<max_node+2;i++){  //Output route
        			if(finall_line[i]>0){
        				for(int a = 0; a < schedules_number; a++){
        					for(int b = 0; b < stops_line[a].stops_num; b++){
        						if(stops_line[a].mark_number[b] == finall_line[i]){
        							if((strcmp(tempname,stops_line[a].stops_name[b]) == 0)){
        								printf("Change at %s\n", stops_line[a].stops_name[b]);
        							}
        							strcpy(tempname,stops_line[a].stops_name[b]);
        							printf("%s %s\n",stops_line[a].time[b], stops_line[a].stops_name[b]);
        						}
        					}

        				}
        			}
        		}
        		free(finall_line);
        	}else{
        		printf("No connection found.\n");
        	}
        	printf("\n");
        	printf("From: ");
        	scanf("%s", inputfrom);
        }
        for(int a = 0; a < schedules_number; a++){ //all space allocated by free
        	for(int b = 0; b < stops_line[a].stops_num; b++){
        		free(stops_line[a].stops_name[b]);
        		free(stops_line[a].time[b]);
        	}
        	free(stops_line[a].stops_name);
        	free(stops_line[a].time);
        	free(stops_line[a].mark_number);

        }
        free(stops_line);
        freeGraph(goNSW);
    }
    printf("Thank you for using goNSW.");
    printf("\n");
    return 0;
}