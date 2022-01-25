#include <iostream>
#include <map>

class ChocolateBoiler{
private:
   bool _empty;
   bool _boiled;
   ChocolateBoiler(){
       _empty = true;
       _boiled = false;
   }
        ChocolateBoiler(const ChocolateBoiler& root) = delete;
        ChocolateBoiler& operator=(const ChocolateBoiler&) = delete;

public:

        static ChocolateBoiler& getInstance(){
            static ChocolateBoiler theInstance;
            return theInstance;
        }
 
    bool isEmpty() const{
       return _empty;
   }

   bool isBoiled() const{
       return _boiled;
   }

   void fill(){
       if (isEmpty()){
           _empty = false;
           _boiled = false;
       }
   }

   void drain(){
       if (!isEmpty() && isBoiled()){
           _empty = true;
       }
   }

   void boil(){
       if (!isEmpty() && !isBoiled()){
           _boiled = true;
       }
   }
};

//=======================================================================

class ISellSystem{
public:
   virtual void changePrice(std::string, float) = 0;
   virtual void sellProduct(std::string) = 0;
   virtual void addProduct(std::string, size_t) = 0;
};

class Automaton: ISellSystem {
private:
   std::string location;
   double recieved_money;
   double spent_money;
   std::map <std::string, float> goods_price;
   std::map <std::string, float> goods_count;

public:
   Automaton(const std::string &location, double recievedMoney) : location(location), recieved_money(recievedMoney) {
       spent_money = 0.0;
   }

   void changePrice(std::string name, float newPrice) override {
       goods_price[name] = newPrice;
   }

   void sellProduct(std::string name){
       if(goods_count[name] > 0){
           recieved_money += goods_price[name];
       }
       else{
           std::cerr << "Not enough goods" << std::endl;
       }
   }

   void addProduct(std::string name, size_t count){
       goods_count[name] += count;
   }

   double allMoneyReport() const{
       return recieved_money - spent_money;
   }

   const std::string& getLocationReport() const {
       return location;
   }

   void goodsCountReport() const{
       for (auto& [product,count]: goods_count){
           std::cout << product << ": " << count << std::endl;
       }
   };

   void goodsPriceReport() const{
       for (auto& [product,price]: goods_price){
           std::cout << product << ": " << price << std::endl;
       }
   };
};

//=================================================================

class Proxy: ISellSystem {
private:
   std::string location;
   double recieved_money;
   double spent_money;
   std::map <std::string, float> goods_price;
   std::map <std::string, float> goods_count;

   Automaton *_real_Auto;

public:
//   Automaton(const std::string &location, double recievedMoney) : location(location), recieved_money(recievedMoney) {
//       spent_money = 0.0;
//   }

    Proxy(Automaton* real_Auto) : _real_Auto(new Automaton(*real_Auto)){}

   void changePrice(std::string name, float newPrice) override {
       this->_real_Auto->changePrice(name, newPrice);
   }

   void sellProduct(std::string name){
       this->_real_Auto->sellProduct(name);
   }

   void addProduct(std::string name, size_t count){
       this->_real_Auto->addProduct(name, count);
   }

   double allMoneyReport() const{
       return this->_real_Auto->allMoneyReport();
   }

   const std::string& getLocationReport() const {
       return this->_real_Auto->getLocationReport();
   }

   void goodsCountReport() const{
       this->_real_Auto->goodsCountReport();
   }

   void goodsPriceReport() const{
       this->_real_Auto->goodsPriceReport();

   }
};

int main(const int argc, const char **argv){

    Automaton *first = new Automaton("place_1", 10.0);
    Proxy *proxy = new Proxy(first);
    std::cout << "Location " << proxy->getLocationReport() << std::endl;

    return 0;
}

