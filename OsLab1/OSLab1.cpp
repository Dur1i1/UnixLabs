#include <iostream>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
bool ready = false;
bool finalMsg = false;

void* provider(void*) {
    for (int i = 0; i < 10; i++) {
        sleep(1);
        pthread_mutex_lock(&mutex);
        ready = true;
        if (i == 9) finalMsg = true;
        std::cout << "Sent\n";
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

void* consumer(void*) {
    while (true) {
        pthread_mutex_lock(&mutex);
        while (!ready) pthread_cond_wait(&cond, &mutex);
        ready = false;
        std::cout << "Recieved\n";
        if (finalMsg)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

int main() {
    pthread_t prov, cons;
    pthread_create(&prov, nullptr, provider, nullptr);
    pthread_create(&cons, nullptr, consumer, nullptr);
    pthread_join(prov, nullptr);
    pthread_join(cons, nullptr);
    return 0;
}