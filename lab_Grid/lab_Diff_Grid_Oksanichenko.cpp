#include <iostream>

template <typename T>
class Grid {
private:
 T *memory;
 size_t x_size, y_size;
 bool *is_sub;

public:



 Grid(size_t x_size, size_t y_size):x_size(x_size),y_size(y_size)
 {
     memory = new T[x_size*y_size];
     is_sub = new bool[x_size*y_size];
     for (int i = 0; i<x_size*y_size; i++)
     {
         is_sub[i] = false;
     }
 }

 bool is_subrid(size_t x_idx, size_t y_idx) const
 {
     return is_sub[x_idx*y_size+y_idx];
 }

 Grid<T>& make_subgrid(size_t x_idx, size_t y_idx, size_t x_sub_size, size_t y_sub_size)
 {
     if (is_subrid(x_idx, y_idx))
     {
        std::cout << is_subrid(x_idx, y_idx);
        this->collapse_subgrid(x_idx, y_idx, (memory[x_idx*y_size+y_idx]).get_xsize(), (memory[x_idx*y_size+y_idx]).get_ysize());
     }
     Grid<T> g(x_sub_size,y_sub_size);
     g = memory[x_idx*y_size+y_idx];
     memory[x_idx*y_size+y_idx] = g;
     return memory[x_idx*y_size+y_idx];
 }
 template <typename T1>
 Grid<T1>& collapse_subgrid( size_t x_idx, size_t y_idx, size_t x_sub_size, size_t y_sub_size)
 {
    if (is_subrid(x_idx, y_idx))
    {
        T A = 0;
        for (size_t i = 0 ; i<x_sub_size ; i++)
        {
            for (size_t j = 0 ; j<y_sub_size ; j++)
            {
                A+=memory[x_idx*y_size+y_idx](x_idx,y_idx);
            }
        }
        A = A / (x_sub_size*y_sub_size);
        if (is_subrid(x_idx, y_idx))
        {
            delete[] memory[x_idx*y_size+y_idx];
        }
        memory[x_idx*y_size+y_idx] = A;
        return this;
    }
 }

 template <typename T1>
 Grid<T1>& get_subgrid(size_t x_idx, size_t y_idx)
 {
    if (is_subrid(x_idx, y_idx))
    {
        return memory[x_idx*y_size+y_idx];
    }
    else
    {
        std::cout << "Попытка вызвать обычный элемент вместо сетки";
    }
 }

 template <typename T1>
 Grid<T1> const& get_subgrid(size_t x_idx, size_t y_idx)
 {
    if (is_subrid(x_idx, y_idx))
    {
        return memory[x_idx*y_size+y_idx];
    }
    else
    {
        std::cout << "Попытка вызвать обычный элемент вместо сетки";
    }
 }

 T operator()(size_t x_idx, size_t y_idx) const
 {
    if (is_subrid(x_idx, y_idx))
    {
        std::cout << "Попытка вызвать обычным обращением сетку";
    }
    else
    {
        return memory[x_idx*y_size+y_idx];
    }

 }

 T& operator()(size_t x_idx, size_t y_idx)
 {
    if (is_subrid(x_idx, y_idx))
    {
        std::cout << "Попытка вызвать обычным обращением сетку";
    }
    else
    {
        return memory[x_idx*y_size+y_idx];
    }

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
    os << std::endl;
    os << std::endl;
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
    Grid<float> g1(3,3);
    std::cout << "AAAAAA";
    g = 1.0;
    g1 = 2.0;
    g.make_subgrid(1,1,2,2);

    std::cout << g << std::endl;
    //std::cin >> g;
    std::cout << g << std::endl;
    //std::cout << g(1,1) << std::endl;
    //std::cout << g(0,0) << std::endl;
    //std::cout << &memory[1];

}
