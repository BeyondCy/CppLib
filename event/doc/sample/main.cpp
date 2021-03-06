// EventHub.cpp : Defines the entry point for the console application.
//

#if defined(_MSC_VER)
#include <event\EventHub.h>
#elif defined(__GNUC__)
#include <event/EventHub.h>
#else
#error unsupported compiler
#endif
#include <iostream>
#include <thread>

std::mutex lock;
class TEST
{
public:
    TEST(int t = 1) : a(t) {}
    void event(u_int event, TEST *t)
    {
        std::unique_lock<std::mutex> lck(lock);
        std::cout << "thread id " << std::this_thread::get_id() << std::endl;
        std::cout << "get event " << event << " a " << t->a << std::endl;
    }
    int a;
};

void event(u_int event, int *s)
{
    std::unique_lock<std::mutex> lck(lock);
    std::cout << "global" << std::endl;
}

void event2(u_int event, void *s)
{
    std::unique_lock<std::mutex> lck(lock);
    std::cout << "global void" << std::endl;
}

int main()
{
    EventHub a(100);
    TEST b(3);
    EventHub::ID id1;
    EventHub::ID id2;
    EventHub::ID id3;
    EventHub::ID id4;
    EventHub::ID id5;

    EventHub::ID id6 = a.SubscribeEvent<TEST>(1, std::bind(&TEST::event, b, std::placeholders::_1, std::placeholders::_2));
    EventHub::ID id7 = a.SubscribeEvent<int>(1, ::event);
    EventHub::ID id8 = a.SubscribeEvent<void>(1, ::event2);
    while (1)
    {
        a.ReSetPool(rand()%100);
        id1 = a.SubscribeEvent<TEST>(1, std::bind(&TEST::event, b, std::placeholders::_1, std::placeholders::_2));
        id2 = a.SubscribeEvent<TEST>(1, std::bind(&TEST::event, b, std::placeholders::_1, std::placeholders::_2));
        id3 = a.SubscribeEvent<TEST>(1, std::bind(&TEST::event, b, std::placeholders::_1, std::placeholders::_2));
        id4 = a.SubscribeEvent<TEST>(1, std::bind(&TEST::event, b, std::placeholders::_1, std::placeholders::_2));
        id5 = a.SubscribeEvent<TEST>(1, std::bind(&TEST::event, b, std::placeholders::_1, std::placeholders::_2));

        a.UnSubscribeEvent(1, std::move(id1));
        a.UnSubscribeEvent(1, std::move(id2));
        a.UnSubscribeEvent(1, std::move(id3));
        a.UnSubscribeEvent(1, std::move(id4));
        a.UnSubscribeEvent(1, std::move(id5));
        //a.UnSubscribeEvent(1, std::move(id6));
        //a.UnSubscribeEvent(1, std::move(id7));
        //new std::thread([&b, &a]
        //{
            auto ret = a.DispatchEvent(1, std::move(b));
            ret = a.DispatchEvent(1, 1);
            if (!ret) std::cout << "DispatchEvent error" << std::endl;
        //});
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}

