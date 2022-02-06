/*
* Домашнее задание к уроку 2
* курса "Паттерны проектирования GOF"
* автор Недокунев А.В.
* среда разработки VSCode /компилятор MinGW/
* codepage UTF-8
*/
#include <iostream>
#include <vector>
#include <list>

/*
class Pizza{
public:
   virtual void prepare(std::string);
   virtual void bake() {};
   virtual void cut() {};
   virtual void box() {};
};
class CheesePizza:public Pizza{};
class GreekPizza:public Pizza{};
class PepperoniPizza:public Pizza{};
Pizza* orderPizza(std::string type){
   Pizza* pizza = nullptr;
   if (type == "cheese"){
       pizza = new CheesePizza;
   } else if (type == "greek"){
       pizza = new GreekPizza;
   } else if (type == "pepperoni"){
       pizza = new PepperoniPizza;
   }
   return pizza;
}

1. Реализуйте паттерн “абстрактная фабрика” который позволит создавать пиццу не привязываясь к её типу, 
чтобы внутри функции orderPizza()  не создавались конкретные экземпляры классов с пиццами
*/
class IPizza{
public:
    virtual void prepare() = 0;
    virtual void bake(){};
    virtual void cut(){};
    virtual void box(){};
    virtual ~IPizza() {};
};

class CheesePizza : public IPizza{
public:
    void prepare() override {
        std::cout << "CheesePizza" << std::endl;
    }
};
class GreekPizza : public IPizza{
public:
    void prepare() override {
        std::cout << "GreekPizza" << std::endl;
    }
};
class PepperoniPizza : public IPizza{
public:
    void prepare() override {
        std::cout << "PepperoniPizza" << std::endl;
    }
};

class IPizzaFactory {
public:
    IPizza* createPizza(std::string type) {
        if (type == "cheese"){
            return createCheesePizza();
        }
        else if (type == "greek"){
            return createGreekPizza();
        }
        else if (type == "pepperoni"){
            return createPepperoniPizza();
        }
        return nullptr;
    }
    virtual ~IPizzaFactory() = default;
protected:
    IPizza* createCheesePizza() {
        return new CheesePizza;
    }
    IPizza* createGreekPizza() {
        return new GreekPizza;
    }
    IPizza* createPepperoniPizza()  {
        return new PepperoniPizza;
    }
};

IPizza* orderPizza(std::string type){
   IPizzaFactory* pPizza = new IPizzaFactory;
   IPizza* pizza = pPizza->createPizza(type);
   delete pPizza;
   return pizza;
};

//=======================================================================

/*
enum class Colors {
   RED,
   ORANGE,
   YELLOW,
   GREEN,
   CYAN,
   BLUE,
   VIOLET,
   WHITE
};

class Light{
public:
   Light():state(false), color(Colors::WHITE){};
   void On(){
       if(!state){
           state = true;
       }
   };
   void Off(){
       if(state){
           state = false;
       }
   };
   void changeColor(Colors newColor){
       color = newColor;
   }
   bool getState() const {
       return state;
   }
private:
   bool state;
   Colors color;
};
2. Реализуйте паттерн команда, чтобы можно было сохранять состояние лампочки 
и проигрывать её мерцание как в прямом так и в обратном направлении.
*/
enum class Colors {
   RED,
   ORANGE,
   YELLOW,
   GREEN,
   CYAN,
   BLUE,
   VIOLET,
   WHITE
};

class Light{
public:
   Light():state(false), color(Colors::WHITE){};

   void On(){
       if(!state){
           state = true;
           std::cout << "light on" << std::endl;
       }
   };

   void Off(){
       if(state){
           state = false;
           std::cout << "light off" << std::endl;
       }
   };

   void changeColor(Colors newColor){
       color = newColor;
   }

   bool getState() const {
       return state;
   }

private:
   bool state;
   Colors color;
};

class Command{
protected:
    Light* _myLight;
public:
    virtual  void lightOn() = 0;
    virtual  void lightOff() = 0;
    virtual  void setLightColor(Colors) = 0;
    virtual  bool getState() = 0;
    virtual  std::string strGetState() = 0;
    virtual ~Command() {};

    void setLight(Light* myLight){
        _myLight = myLight;
    }
};

class LightCommand : public Command{
private:
    bool _state;
public:
    void lightOn() override{
        _myLight->On();
        _state = _myLight->getState();
    }
    void lightOff() override{
        _myLight->Off();
        _state = _myLight->getState();
    }
    void setLightColor(Colors color) override{
        _myLight->changeColor(color);
    }
    bool getState() override{
        return _state;
    }
    std::string strGetState() override{
        if (_state) return "Light ON";
        else return "Light OFF";
    }
};

class Invoker{
    std::list<Command*> DoneCommands;
    Light light;
    Command* pCommand;
    std::list<Command*>::const_iterator it; // Итератор текущей позиции при перемещении по истории

public:
    Invoker() : pCommand(nullptr), it(DoneCommands.end()) {
    }
    ~Invoker(){
        for(Command* ptr: DoneCommands){
            delete ptr;
        }
    }
    void On(){
        if(!light.getState()) {
            pCommand = new LightCommand();
            pCommand->setLight(&light);
            pCommand->lightOn();
            DoneCommands.push_back(pCommand);
            it = DoneCommands.end();
        }
    }
    void Off(){
        if(light.getState()) {
            pCommand = new LightCommand();
            pCommand->setLight(&light);
            pCommand->lightOff();
            DoneCommands.push_back(pCommand);
            it = DoneCommands.end();
        }
    }
    void changeColor(Colors color){
        pCommand = new LightCommand();
        pCommand->setLight(&light);
        pCommand->setLightColor(color);
    }
    void next(){
        if (DoneCommands.size() != 0){
           if(it == DoneCommands.end()) {
               std::cerr << "No NEXT command" << std::endl;
           } else {
               ++it;
               std::cout << "Current state: " << (*it)->strGetState() << " (" << (*it)->getState() << ")" << std::endl;
           }

        }
        else{
            std::cerr << "No any commands (empty)" << std::endl;
        }
    }
    void prev(){
        if (DoneCommands.size() != 0){
           if(it == DoneCommands.begin()) {
               std::cerr << "No PREV command" << std::endl;
           } else {
               --it;
               std::cout << "Current state: " << (*it)->strGetState() << " (" << (*it)->getState() << ")" << std::endl;
           }

        }
        else{
            std::cerr << "No any commands (empty)" << std::endl;
        }
    }
};

//=======================================================================
/*
class Beverage{            // Абстрактный класс
public:
   virtual std::String getDescription() const;
   virtual double cost() const = 0;
   virtual ~Beverage() {}
};
class HouseBlend: public Beverage{   // Молотый
public:
   double cost() const override { return 5.5; }
};
class DarkRoast: public Beverage{  // Темной обжарки
public:
   double cost() const override { return 6.0; }
};
class Decaf: public Beverage{    // Без кофеина
public:
   double cost() const override { return 7.3; }
};
class Espresso: public Beverage{  // Эспрессо
public:
   double cost() const override { return 3.5; }
};
Реализуйте паттерн декоратор с помощью которого можно будет сварить кофе с любым составом добавок.
*/

class Beverage{            // Абстрактный класс
public:
    virtual std::string getDescription() const = 0;
    virtual double cost() const = 0;
    virtual ~Beverage() {};
};

class HouseBlend: public Beverage{   // Молотый
public:
    virtual std::string getDescription() const override {
        return "HouseBlend";
    }
    double cost() const override {
        return 5.5;
    }
};

class DarkRoast: public Beverage{  // Темной обжарки
public:
    virtual std::string getDescription() const override {
        return "DarkRoast";
    }
    double cost() const override {
        return 6.0;
    }
};

class Decaf: public Beverage{    // Без кофеина
public:
    virtual std::string getDescription() const override {
        return "Decaf";
    }
    double cost() const override {
        return 7.3;
    }
};

class Espresso: public Beverage{  // Эспрессо
public:
    virtual std::string getDescription() const override {
        return "Espresso";
    }
    double cost() const override {
        return 3.5;
    }
};

class Decorator: public Beverage{
public:
    Decorator(Beverage* beverage): _beverage(beverage){};
    virtual std::string getDescription() const override {
        return _beverage->getDescription();
    }
    virtual double cost() const override {
        return _beverage->cost();
    }

private:
    Beverage* _beverage;
};

class addChocolate : public Decorator { // шоколад
public:
    addChocolate(Beverage* beverage) : Decorator(beverage) {}
    virtual double cost() const override {
        return Decorator::cost() + 1.0;
    }
    virtual std::string getDescription() const override {
        return Decorator::getDescription() + " with Chocolate";
    }
};
class addCinnamon : public Decorator { // корица
public:
    addCinnamon(Beverage* beverage) : Decorator(beverage) {}
    virtual double cost() const override {
        return Decorator::cost() + 1.5;
    }
    virtual std::string getDescription() const override {
        return Decorator::getDescription() + " with Cinnamon";
    }
};
class addCream : public Decorator { // сливки
public:
    addCream(Beverage* beverage) : Decorator(beverage) {}
    virtual double cost() const override {
        return Decorator::cost() + 0.5;
    }
    virtual std::string getDescription() const override {
        return Decorator::getDescription() + " with Cream";
    }
};
class withoutSugar : public Decorator { // без сахара
public:
    withoutSugar(Beverage* beverage) : Decorator(beverage) {}
    virtual double cost() const override {
        return Decorator::cost() - 1.2;
    }
    virtual std::string getDescription() const override {
        return Decorator::getDescription() + " without Sugar";
    }
};

//===========================================================

int main(const int argc, const char **argv){

    {
    std::cout << "===================== 1 =====================" << std::endl;
    IPizza* myPizza = orderPizza("cheese");    
    myPizza->prepare();
    myPizza = orderPizza("greek");
    myPizza->prepare();
    myPizza = orderPizza("pepperoni");
    myPizza->prepare();
    delete myPizza;
    }
//====================================================
    {
    std::cout << "===================== 2 =====================" << std::endl;

    Invoker inv;

    inv.On();
    inv.Off();
    inv.On();
    inv.On();
    inv.Off();
    inv.On();
    inv.changeColor(Colors::GREEN);

    inv.next();
    inv.prev();
    inv.prev();
    inv.prev();
    inv.prev();
    inv.prev();
    inv.prev();
    inv.Off();
    inv.prev();
    inv.prev();
    }
//===================================================
    {
    std::cout << "===================== 3 =====================" << std::endl;

    Espresso* coffee = new Espresso();
    std::cout << coffee->getDescription() << " : " << coffee->cost() << std::endl;

    Decorator* cream = new addCream(coffee);
    std::cout << cream->getDescription() << " : " << cream->cost() << std::endl;

    Decorator* cinnamon = new addCinnamon(cream);
    std::cout << cinnamon->getDescription() << " : " << cinnamon->cost() << std::endl;

    Decorator* ws = new withoutSugar(cinnamon);
    std::cout << ws->getDescription() << " : " << ws->cost() << std::endl;

    delete ws;
    delete cinnamon;
    delete cream;
    delete  coffee;
    }
//====================================================
    int a;
    std::cin >> a; 

    return 0;
}