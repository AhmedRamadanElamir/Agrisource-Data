#ifndef DHT11_h
#define DHT11_h


class fclass
{
  public:
    fclass();
    void dht_init();
    char read_data();
    void start_test();
    char get_Temp();
    char get_Hum();

};

extern fclass dht;

#endif
