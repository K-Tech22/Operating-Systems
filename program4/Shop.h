#ifndef SHOP_ORG_H_
#define SHOP_ORG_H_
#include <pthread.h>
#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>

using namespace std;

#define kDefaultNumChairs 3
#define kDefaultNumBarbers 1

class Shop
{
public:

    Shop(int num_barbers, int num_chairs) : max_waiting_cust_((num_chairs > 0) ? num_chairs : kDefaultNumChairs), cust_drops_(0), num_barbers_((num_barbers > 0) ? num_barbers : kDefaultNumBarbers)
    {
        init();
    };

    Shop() : max_waiting_cust_(kDefaultNumChairs), cust_drops_(0), num_barbers_(1)
    {
        init();
    };


    int visitShop(int id); // return true only when a customer got a service

    void leaveShop(int customer_id_, int barber_id_);
    
    void helloCustomer(int barber_id_);
    
    void byeCustomer(int barber_id_);
    
    int get_cust_drops() const;

private:
    
    const int max_waiting_cust_; // the max number of threads that can wait

    int num_barbers_; // barbers >= 1
    
    queue<int> waiting_chairs_; // includes the ids of all waiting threads
    
    int cust_drops_;

    struct condition_variables {

        condition_variables() {
            pthread_cond_init(&cond_customer_served_, NULL);
            pthread_cond_init(&cond_barber_paid_, NULL);
            pthread_cond_init(&cond_barber_sleeping_, NULL);
        }
        pthread_cond_t cond_customer_served_;
        pthread_cond_t cond_barber_paid_;
        pthread_cond_t cond_barber_sleeping_;

        int customer_in_chair_ = 0;
        bool in_service_ = false;
        bool money_paid_ = false;
        
    };
    
    // Mutexes and condition variables to coordinate threads
    
    // mutex_ is used in conjuction with all conditional variables
    
    pthread_mutex_t mutex_;
    
    pthread_cond_t cond_customers_waiting_;

    vector<condition_variables*> conditions;
    
    static const int barber = 0; // the id of the barber thread
    
    void init();
    
    string int2string(int i);
    
    void print(int person, string message);

    void print_barber(int barber, string message);

    void print_person(int person, string message);

    bool isBarberAvail();
};
#endif
