//
//  main.c
//  tcp_bench
//
//  Created by green on 23/11/2016.
//  Copyright © 2016 green. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "thpool.h"

#define SERVER_PORT 5222
#define MAXBUF 1024
#define NUMTHREADS 300
#define NUMREQUESTS 1000

//#define PRINT_DEBUG

// the thread function
void client_worker();

char server_addr[16];

int main(int argc, const char *argv[]) {

  if (argc != 2) {
    puts("Usage: ./tcp_bench 1.2.3.4");
    exit(EXIT_FAILURE);
  }

  strcpy(server_addr, argv[1]);
  
  // get limits
  struct rlimit rln;
  getrlimit(RLIMIT_NOFILE, &rln);
  printf("NOFILE SOFT LIMIT: %d\n", (int)rln.rlim_cur);
  struct rlimit rlp;
  getrlimit(RLIMIT_NPROC, &rlp);
  printf("NPROC SOFT LIMIT: %d\n", (int)rlp.rlim_cur);
  
  printf("target: %s:%d\n", server_addr, SERVER_PORT);
  printf("Will send %d requests using %d threads pool\n", NUMREQUESTS, NUMTHREADS);
    
  printf("Making thread pool with %d threads\n", NUMTHREADS);
  threadpool thpool = thpool_init(NUMTHREADS);
    
  printf("Adding %d tasks to thread pool\n", NUMREQUESTS);

  struct timespec start_time;
  clock_gettime(CLOCK_REALTIME, &start_time);

  int i;
  for (i=0; i<NUMREQUESTS; i++){
    thpool_add_work(thpool, client_worker, NULL);
  };
    
  thpool_wait(thpool);
    
  struct timespec end_time;
  clock_gettime(CLOCK_REALTIME, &end_time);
  printf("test took %f seconds\n",
         ((end_time.tv_sec * 1.0e9 + end_time.tv_nsec) - (start_time.tv_sec * 1.0e9 + start_time.tv_nsec)) / 1.0e9);
    
  puts("Killing thread pool");
  thpool_destroy(thpool);
    
  /*
    // without prethreaded pool
    pthread_t thread[NUMTHREADS];

    unsigned long start_time = time(NULL);
    printf("START: %lu\n", start_time);
    
    int i;
    for (i = 0; i < NUMTHREADS; i++) {
    int *args = malloc(sizeof(int));
    *args = i;
        
    if (pthread_create(&thread[i], NULL, client_worker, args) < 0) {
    free(args);
    perror("Could not create thread");
    exit(errno);
    }
    }
    
    for (i = 0; i < NUMTHREADS; i++) {
    pthread_join(thread[i], NULL);
    }

    unsigned long end_time = time(NULL);
    printf("END: %lu (%lu)\n", end_time, end_time - start_time);
  */

  return 0;
}

void client_worker() {
  int sock;
  struct sockaddr_in dst;
  char message[MAXBUF];
  char server_reply[MAXBUF];
    
  // init server address struct
  bzero(&dst, sizeof(dst));
  dst.sin_family = AF_INET;
  dst.sin_port = htons(SERVER_PORT);
  if (inet_aton(server_addr, &dst.sin_addr) == 0) {
    printf("Wrong IP address: %s\n", server_addr);
    return;
  }
    
  // open socket for streaming
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket");
    return;
  }
    
  // connect to server
  if (connect(sock, (struct sockaddr*)&dst, sizeof(dst)) != 0) {
    perror("Connect");
    return;
  }
    
  // send a message
  //strcpy(message, "GET / HTTP/1.1\r\nHost: bars.kh.ua\r\n\r\n");
  strcpy(message, "<stream:stream from='%d-1@chat.banuba.com' to='chat.banuba.com' version='1.0' xml:lang='en' xmlns='jabber:client' xmlns:stream='http://etherx.jabber.org/streams'>");
  if (send(sock, message, strlen(message), 0) < 0) {
    perror("Send");
    return;
  }
#ifdef PRINT_DEBUG
  printf("%i: Sent %lu bytes\n", (int)pthread_self(), strlen(message));
#endif

  // get the answer
  bzero(server_reply, MAXBUF);
  if (recv(sock, server_reply, MAXBUF, 0) < 0) {
    perror("Read");
    return;
  }
#ifdef PRINT_DEBUG
  printf("%i: Received %lu bytes\n", (int)pthread_self(), strlen(server_reply));
  printf("%s\n", server_reply);
#endif

  close(sock);
    
  return;
}
