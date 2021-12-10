#include <iostream>

template <typename T>
class Grid {
private:
 T *memory;
 size_t x_size, y_size;

public:
 Grid(size_t x_size, size_t y_size):x_size(x_size),y_size(y_size)
 {
     memory = new T[x_size*y_size];

 }

 Grid<T> (const Grid<T>& G)
 {
     this->x_size = G.get_xsize();
     this->y_size = G.get_ysize();
     this->memory = new T[x_size*y_size];
     for(int i = 0;i<x_size*y_size;i++)
     {
         this->memory[i] = G.memory[i];
     }
 }

 T operator()(size_t x_idx, size_t y_idx) const
 {
    return memory[x_idx*y_size+y_idx];

 }

 T& operator()(size_t x_idx, size_t y_idx)

 {
    return memory[x_idx*y_size+y_idx];
 }

 size_t get_xsize() const
 {
     return this->x_size;
 }
 size_t get_ysize() const
 {
     return this->y_size;
 }

 Grid& operator=(T A)
 {
     for (size_t i=0;i<x_size*y_size;i++){
         memory[i] = A;
     }
     return *this;
 }
 Grid& operator=(Grid<T> G)
 {
     for (size_t i=0;i<x_size*y_size;i++){
         this->memory[i] = 0;
     }
     size_t x_s = this->get_xsize();
     if (x_s > G.get_xsize())
     {
         x_s = G.get_xsize();
     }
     size_t y_s = this->get_ysize();
     if (y_s > G.get_ysize())
     {
         y_s = G.get_ysize();
     }

     for (size_t i=0;i<x_s;i++){
         for (size_t j = 0; j < y_s;j++)
         {
             this->memory[i*this->y_size+j] = G.memory[i*G.get_ysize()+j];
         }
     }
     return *this;
 }
 template <typename T1>
 friend std::ostream& operator<<(std::ostream&, Grid<T1> const&);
 template <typename T2>
 friend std::istream& operator>>(std::istream&, Grid<T2> &);

 ~Grid()
 {
     delete[] memory;
 }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, Grid<T> const& g)
{
    os << std::endl;
    for (size_t i = 0 ; i<g.get_xsize() ; i++)
    {
        for (size_t j = 0 ; j<g.get_ysize() ; j++)
        {
            os << g(i,j);
            if (j!=g.get_ysize()-1)
            {
                os << " - ";
            }

        }
        os << std::endl;
    }
    return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, Grid<T>
                         & g)
{
    for (size_t i = 0 ; i<g.get_xsize() ; i++)
    {
        for (size_t j = 0 ; j<g.get_ysize() ; j++)
        {
            T curr;
            is >> curr;
            g(i,j) = curr;
        }
    }
    return is;
}


int main()
{
    std::cout << "ddfdfd";
    Grid<float> g(3,3);
    std::cout << "AAAAAA";
    g = 1.0;
    g(1,1) = 2.0;

    Grid<float> g1(4,4);
    g1 = g;

    Grid<float> g2(g);

    {
        Grid<float> g2(1,1);
    }

    std::cout << g2 << std::endl;
    //std::cin >> g;
    std::cout << g1 << std::endl;
    //std::cout << g(1,1) << std::endl;
    //std::cout << g(0,0) << std::endl;
    //std::cout << &memory[1];

}


