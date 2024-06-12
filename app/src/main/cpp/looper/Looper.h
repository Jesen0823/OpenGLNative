//
// Created by xie_s on 2024/6/12.
//

#ifndef OPENGLNATIVE_LOOPER_H
#define OPENGLNATIVE_LOOPER_H

#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>

struct LooperMessage {
    int what;
    int arg1;
    int arg2;
    void *obj;
    LooperMessage *next;
    bool quit;
};

class Looper {

public:
    Looper();

    Looper &operator=(const Looper &) = delete;

    Looper(Looper &) = delete;

    virtual ~Looper();

    void postMessage(int what, bool flush = false);

    void postMessage(int what, void *obj, bool flush = false);

    void postMessage(int what, int arg1, int arg2, bool flush = false);

    void postMessage(int what, int arg1, int arg2, void *obj, bool flush = false);

    void quit();

    virtual void handleMessage(LooperMessage *msg);

private:
    void addMessage(LooperMessage *msg, bool flush);

    static void *trampoline(void *p);

    void loop(void);

    LooperMessage *head;
    pthread_t worker;
    sem_t headWriteProtect;
    sem_t headDataAvailable;
    bool running;
};


#endif //OPENGLNATIVE_LOOPER_H
