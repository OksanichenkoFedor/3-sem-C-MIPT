#include <iostream>
#include <vector>
#include <string>
#include <math.h>
using namespace std;


struct vector3 {

public:
    double x;
    double y;
    double z;
    vector3(double X, double Y, double Z){
        x = X;
        y = Y;
        z = Z;
    }
};

// Для описания событий происходящих в мире напишите свой небольшой логгер, который умеет
// Запоминать создание, изменение и уничтожение частиц

class Logger {
public:
    Logger()
    {

    }
    void log(const std::string& s)
    {
        this->l.push_back(s);
    }
    void show()
    {
        for (int i = 0; i<this->l.size(); i++){
            cout << l[i] << endl;
        }
    }
    ~Logger(){}
private:
    vector<string> l;
};

// Существует базовый класс частиц, который внутри себя хранит информацию о частице
// Допишите этот родительский класс так, чтобы он мог считать количество объектов, созданных от него

class Particle {

private:
    static int num;

protected:
    // Поля, описывающие частицу
    // Можно дописать свои
    double mass = 0;
    double charge = 0;
    double energy = 0;
    int id;
    // Логгер для записи событий
    Logger logger;

public:
    vector3 coordinate = {0, 0, 0};
    vector3 velocity = {0, 0,0};
    vector3 force = {0, 0, 0};
    string name;
    explicit Particle(double curr_energy, string curr_name)
    {

        this->name = curr_name;
        this->energy = curr_energy;
        logger.log("Birth of "+name);
        num+=1;
        this->id = num;
    }
    virtual void interaction(Particle& p)
    {
        double F = (9e10)*(this->charge)*(p.charge)/(pow(this->coordinate.x-p.coordinate.x,2)+pow(this->coordinate.y-p.coordinate.y,2)+pow(this->coordinate.z-p.coordinate.z,2));
        double r = pow((pow(this->coordinate.x-p.coordinate.x,2)+pow(this->coordinate.y-p.coordinate.y,2)+pow(this->coordinate.z-p.coordinate.z,2)),0.5);
        this->force.x+=(F*(this->coordinate.x-p.coordinate.x)/r);
        this->force.y+=(F*(this->coordinate.y-p.coordinate.y)/r);
        this->force.z+=(F*(this->coordinate.z-p.coordinate.z)/r);
        p.force.x-=(F*(this->coordinate.x-p.coordinate.x)/r);
        p.force.y-=(F*(this->coordinate.y-p.coordinate.y)/r);
        p.force.z-=(F*(this->coordinate.z-p.coordinate.z)/r);
        logger.log("Interacting with "+p.name);
        //p.logger.log("Interaction with "+this.name);
    }
    string getName() const
    {
        return this->name;
    }
    double getEnergy() const
    {
        return this->energy;
    }
    static double getNum()
    {
        return num;
    }
    void setCharge(double new_charge)
    {
        this->charge = new_charge;
    }
    double getCharge()
    {
        return this->charge;
    }
    bool operator == (const Particle& p) const
    {
        return (this->id == p.id);
    }
    bool operator != (const Particle& p) const
    {
        return (this->id != p.id);
    }

    ~Particle()
    {
        cout << this->name << " deleted" << endl;
    }
};

ostream& operator<<(ostream& os, const Particle &p)
{
        return os << p.name << endl << "Coordinates: " << p.coordinate.x << " , " << p.coordinate.y << " , " << p.coordinate.z << endl << "Velocity: " << p.velocity.x << " , " << p.velocity.y << " , " << p.velocity.z << endl << "Force: " << p.force.x << " , " << p.force.y << " , " << p.force.z << endl << endl;
}
int Particle::num = 0;

class Electron : public Particle {
public:
    // в этом конструкторе предлагается задавать координаты и через энергию остальные параметры частицы
    explicit Electron(double curr_energy, vector3 coords):Particle(curr_energy, "Electron")
    {
        this->coordinate = coords;
        this->charge = -1.6e-19;
        this->mass = 9.1e-31;
        this->velocity.x = (1.0/3.0)*sqrt(2*(this->energy)/(this->mass));
        this->velocity.y = (1.0/3.0)*sqrt(2*(this->energy)/(this->mass));
        this->velocity.z = (1.0/3.0)*sqrt(2*(this->energy)/(this->mass));
    }
    // Вообще все силы будут одинаковыми так что давай-те не будем переопределять
    //(магнитных зарядов не сущетвует, поэтому не будем их добавлять, а для диполей
    // нам не хватит функционала (нужно будет ещё вектор диполя определять))
};

class Proton : public Particle {
public:
    explicit Proton(double curr_energy, vector3 coords):Particle(curr_energy, "Proton")
    {
        this->coordinate = coords;
        this->charge = 1.6e-19;
        this->mass = 1839*(9.1e-31);
        this->velocity.x = (1.0/3.0)*sqrt(2*(this->energy)/(this->mass));
        this->velocity.y = (1.0/3.0)*sqrt(2*(this->energy)/(this->mass));
        this->velocity.z = (1.0/3.0)*sqrt(2*(this->energy)/(this->mass));

    }
    // Вообще все силы будут одинаковыми так что давай-те не будем переопределять
    //(магнитных зарядов не сущетвует, поэтому не будем их добавлять, а для диполей
    // нам не хватит функционала (нужно будет ещё вектор диполя определять))
};

class Neutron : public Particle {
public:
    // в этом конструкторе предлагается задавать координаты и через энергию остальные параметры частицы
    explicit Neutron(double curr_energy, vector3 coords):Particle(curr_energy, "Neutron")
    {
        this->coordinate = coords;
        this->charge = 0;
        this->mass = 1839*(9.1e-31);
        this->velocity.x = (1.0/3.0)*sqrt(2*(this->energy)/(this->mass));
        this->velocity.y = (1.0/3.0)*sqrt(2*(this->energy)/(this->mass));
        this->velocity.z = (1.0/3.0)*sqrt(2*(this->energy)/(this->mass));

    }
    // Вообще все силы будут одинаковыми так что давай-те не будем переопределять
    //(магнитных зарядов не сущетвует, поэтому не будем их добавлять, а для диполей
    // нам не хватит функционала (нужно будет ещё вектор диполя определять))
};

class World {
public:
    std::vector<Particle> v;
    void addParticle(Particle& p)
    {
        for (int i = 0;i < v.size();i++){
            v[i].interaction(p);
        }
        v.push_back(p);
    }
    void deleteLastParticle(){
        // Я не придумал как удалить частицу, кроме как последнюю
        if (v.size()>0)
        {
            // сначала обнулим силы, с которыми частица действовала на остальные, а потом уберём её
            v[v.size()-1].setCharge((-1.0)*v[v.size()-1].getCharge());
            for (int i = 0; i<v.size()-1;i++)
            {
                v[i].interaction(v[v.size()-1]);
            }
            v.pop_back();
        }
    }
    void deleteParticle(const Particle& p)
    {
        if (v.size()>0)
        {
            for (int i = v.size()-1;i>=0;i--)
            {
                if (v[i]==p){
                    v[i].setCharge((-1.0)*v[i].getCharge());
                    for (int j = 0; j<v.size();j++)
                    {
                        if(j!=i){
                            v[j].interaction(v[i]);
                        }
                    }
                    v.erase(v.begin() + i);
                }
            }
        }
    }
};

ostream& operator<< (ostream& os, const World &w)
{
        for (int i = 0; i<w.v.size();i++)
        {
            os<< w.v[i] << endl;
        }
        return os;
}



// здесь создайте мир/миры, куда добавьте некоторое число частиц, взаимодействующих друг с другом
int main() {
    cout << Particle::getNum() << endl;
    Electron e1 = Electron(10,vector3(1,2,3));
    Electron e2 = Electron(1,vector3(0,0,0));
    Proton p1 = Proton(100,vector3(1,2,4));
    Neutron n1 = Neutron(0,vector3(10,10,10));
    World w1 = World();
    w1.addParticle(e1);
    cout << w1 << endl;
    w1.addParticle(p1);
    cout << w1 << endl;
    w1.deleteParticle(e1);
    cout << w1 << endl;
    cout << "Number of ever created particles: " << Particle::getNum() << endl;
}
