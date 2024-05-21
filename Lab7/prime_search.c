#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_THREADS 1000
#define MAX_PRIMES 10000

typedef struct {
    long long id; 
    long long num_primes; 
    long long lower_bound;
    long long upper_bound;
    long long primes[MAX_PRIMES];
} ThreadData;

ThreadData thread_data[MAX_THREADS];
FILE* log_file;
long long* primes;
long long num_primes = 0;
pthread_mutex_t lock;

bool is_prime(long long n) {
    if (n <= 1) return false;
    for (long long i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

int cmp(const void* left, const void* right){
  long long value1 = *(long long*)left, value2 = *(long long*)right;
  if (value1 > value2) return -1;
  else if (value1 < value2) return 1;
  else return 0;
}

void* worker_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    data->num_primes = 0;

    for (long long i = data->lower_bound; i <= data->upper_bound; i++) {
        if (is_prime(i)) {
	    pthread_mutex_lock(&lock); 
            data->primes[data->num_primes++] = i;
	    primes[num_primes++] = i;
            fprintf(log_file, "Thread %lld found prime: %lld\n", data->id, i);
	    pthread_mutex_unlock(&lock); 
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <num_threads> <lower_bound> <upper_bound>\n", argv[0]);
        return 1;
    }

    long long num_threads = atoll(argv[1]);
    long long lower_bound = atoll(argv[2]);
    long long upper_bound = atoll(argv[3]);

    primes = (long long*)malloc(upper_bound * sizeof(long long));
    

    if (num_threads <= 0 || num_threads > MAX_THREADS) {
        fprintf(stderr, "Invalid number of threads\n");
        return 1;
    }

    if (lower_bound < 0 || upper_bound < lower_bound) {
        fprintf(stderr, "Invalid bounds\n");
        return 1;
    }

    log_file = fopen("primes.log", "w");
    if (log_file == NULL) {
        perror("Error opening log file");
        return 1;
    }
    
    if (pthread_mutex_init(&lock, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    } 

    pthread_t threads[num_threads];
    long long segment_size = (upper_bound - lower_bound + 1) / num_threads;
    for (long long i = 0; i < num_threads; i++) {
        thread_data[i].id = i;
        thread_data[i].lower_bound = lower_bound + i * segment_size;
        thread_data[i].upper_bound = (i == num_threads - 1) ? upper_bound : lower_bound + (i + 1) * segment_size - 1;
        pthread_create(&threads[i], NULL, worker_thread, (void*)&thread_data[i]);
    }

    for (long long i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    fclose(log_file);
    pthread_mutex_destroy(&lock); 

    printf("Statistic:\n");
    for (long long i = 0; i < num_threads; i++) {
        printf("Thread %lld found %lld primes\n", thread_data[i].id, thread_data[i].num_primes);
    }

    printf("\nFound primes:\n");
    qsort(primes, num_primes, sizeof(long long), &cmp);
    for (int i = 0; i < num_primes; i++){
      printf("%lld ", primes[i]);
    }
    printf("\n");
    return 0;
}
