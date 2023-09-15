#include "Shop.h"

void Shop::init()
{
    pthread_mutex_init(&mutex_, NULL);
    pthread_cond_init(&cond_customers_waiting_, NULL);

    int i;
    for (i = 0; i < this->num_barbers_; i++) {
        condition_variables* new_condition_ = new condition_variables();
        conditions.push_back(new_condition_);
    }
}

bool Shop::isBarberAvail(){
    for (int i = 0; i < num_barbers_; i++) {
        if (conditions[i]->customer_in_chair_ == 0) {
            return true;
        }
    }
    return false;
}

string Shop::int2string(int i)
{
    stringstream out;
    out << i;
    return out.str( );
}

void Shop::print(int person, string message)
{
    cout << ((person != barber) ? "customer[" : "barber [" ) << person << "]: " <<
    message << endl;
}

void Shop::print_barber(int barber, string message)
{
    cout << "barber[" << barber << "]: " << message << endl;
}

int Shop::get_cust_drops() const
{
    return cust_drops_;
}

// Customer visits the shop, looks for waiting waiting or the service chair, and then goes to the barber service chair
int Shop::visitShop(int id)
{
    pthread_mutex_lock(&mutex_);
    
    // If all chairs are full then leave shop
    if (waiting_chairs_.size() == max_waiting_cust_ && !isBarberAvail())
    {
        print( id,"leaves the shop because of no available waiting chairs.");
        ++cust_drops_;
        pthread_mutex_unlock(&mutex_);
        return -1;
    }

    // If someone is being served or transitioning waiting to service chair
    // then take a chair and wait for service
    if (!isBarberAvail())
    {
        waiting_chairs_.push(id);

        print(id, "takes a waiting chair. # waiting seats available = " +
        int2string(max_waiting_cust_ - waiting_chairs_.size()));

        while (!isBarberAvail()) {
            pthread_cond_wait(&cond_customers_waiting_, &mutex_);
        }
        
        waiting_chairs_.pop();
    }
    int my_barber_ = -1;

    // Get the customer to their barber
    for(int i = 0; i < num_barbers_; i++) {

        if(conditions[i]->customer_in_chair_ == 0){

            print(id, "moves to the service chair [" + to_string(i) + "]. # waiting seats available = " + int2string(max_waiting_cust_ - waiting_chairs_.size()));
            
            conditions[i]->customer_in_chair_ = id;

            conditions[i]->in_service_ = true;

            pthread_cond_signal(&conditions[i]->cond_barber_sleeping_);
            my_barber_ = i;
            break;
        }
    }
    pthread_mutex_unlock(&mutex_);

    return my_barber_;

}

void Shop::leaveShop(int customer_id_, int barber_id_)
{
    pthread_mutex_lock( &mutex_ );
    // Wait for service to be completed
    string message = "wait for the barber[" + to_string(barber_id_) + "] to be done with hair-cut";
    print(customer_id_, message);

    while (conditions[barber_id_]->in_service_ == true)
    {
        pthread_cond_wait(&conditions[barber_id_]->cond_customer_served_, &mutex_);
    }

    // Pay the barber and signal barber appropriately
    conditions[barber_id_]->money_paid_ = true;
    pthread_cond_signal(&conditions[barber_id_]->cond_barber_paid_);
    print( customer_id_, "says good-bye to the barber." );
    pthread_mutex_unlock(&mutex_);
}

// Barber welcomes the customer, if there are no customers, he goes to sleep, or else gives the haircut. 
void Shop::helloCustomer(int barber_id_)
{
    pthread_mutex_lock(&mutex_);
    // If no customers than barber can sleep
    if (waiting_chairs_.empty() && conditions[barber_id_]->customer_in_chair_ == 0 )
    {
        print_barber(barber_id_, "sleeps because of no customers.");
        
        pthread_cond_wait(&conditions[barber_id_]->cond_barber_sleeping_, &mutex_);
    }

    if (conditions[barber_id_]->customer_in_chair_ == 0) // check if the customer, sit down.
    {
        pthread_cond_wait(&conditions[barber_id_]->cond_barber_sleeping_, &mutex_);
    }
    
    print_barber(barber_id_, "starts a hair-cut service for " +
    int2string( conditions[barber_id_]->customer_in_chair_ ) );
    
    pthread_mutex_unlock( &mutex_ );
}


void Shop::byeCustomer(int barber_id_)
{
    pthread_mutex_lock(&mutex_);
    // Hair Cut-Service is done so signal customer and wait for payment
    
    conditions[barber_id_]->in_service_ = false;
    
    print_barber(barber_id_, "says he's done with a hair-cut service for " +
    int2string(conditions[barber_id_]->customer_in_chair_));
    
    conditions[barber_id_]->money_paid_ = false;
    
    pthread_cond_signal(&conditions[barber_id_]->cond_customer_served_);
    
    while (conditions[barber_id_]->money_paid_ == false)
    {
        pthread_cond_wait(&conditions[barber_id_]->cond_barber_paid_, &mutex_);
    }
    
    //Signal to customer to get next one
    conditions[barber_id_]->customer_in_chair_ = 0;
    
    print_barber(barber_id_, "calls in another customer");
    
    pthread_cond_signal( &cond_customers_waiting_ );
    
    pthread_mutex_unlock( &mutex_ ); // unlock
}
