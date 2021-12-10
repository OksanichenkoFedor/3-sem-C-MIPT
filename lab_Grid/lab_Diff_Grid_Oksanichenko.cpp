#include<iostream>

template <typename T>
class Grid {
private:
 Grid *memory;
 T curr_value;
 size_t x_size, y_size;
 bool is_sub_grid = false;

public:
    Grid(): Grid(0, 0) { }



    Grid(size_t x_size, size_t y_size): curr_value(T()), x_size(x_size), y_size(y_size) {
        if(x_size==0) {
            memory=nullptr;
        }else{
             memory = new Grid[x_size*y_size];
             is_sub_grid = true;
        }

    }

    //правило пяти

    ~Grid() { delete[] memory; }

    Grid(Grid const &curr_grid): Grid(curr_grid.x_size, curr_grid.y_size) {
        curr_value = curr_grid.curr_value;
        is_sub_grid = curr_grid.is_sub_grid;
        if(curr_grid.is_sub_grid){
            for(size_t i=0; i!=x_size*y_size; ++i) {
                    memory[i] = curr_grid.memory[i];
            }
        }


    }

    Grid(Grid &&curr_grid): Grid(curr_grid.x_size, curr_grid.y_size) {
        curr_value = curr_grid.curr_value;
        memory = curr_grid.memory;
        is_sub_grid = curr_grid.is_sub_grid;
        curr_grid.curr_value = T();
        curr_grid.x_size = 0;
        curr_grid.y_size = 0;
        curr_grid.is_sub_grid = false;
        curr_grid.memory = nullptr;
    }

    Grid& operator=(Grid &&curr_grid) {
        Grid temp_grid(std::move(curr_grid));
        std::swap(this->curr_value, temp_grid.curr_value);
        std::swap(this->memory, temp_grid.memory);
        std::swap(this->x_size, temp_grid.x_size);
        std::swap(this->y_size, temp_grid.y_size);
        std::swap(this->is_sub_grid, temp_grid.is_sub_grid);
        return *this;
    }

    Grid& operator=(Grid const &curr_grid) {
        Grid temp_grid(curr_grid);
        std::swap(this->curr_value, temp_grid.curr_value);
        std::swap(this->memory, temp_grid.memory);
        std::swap(this->x_size, temp_grid.x_size);
        std::swap(this->y_size, temp_grid.y_size);
        std::swap(this->is_sub_grid, temp_grid.is_sub_grid);
        return *this;
    }

    //is_subgrid
    bool is_subgrid(size_t x_idx, size_t y_idx) const { return this->is_sub_grid; }


    T operator()(size_t x_idx, size_t y_idx) const {
        if((memory[y_idx*x_size+x_idx]).is_subgrid()) {
            T sum = T();
            size_t curr_pos = y_idx*x_size+x_idx;
            size_t sub_size = (memory[curr_pos]).x_size*(memory[curr_pos]).y_size;
            for(size_t sub_pos=0;sub_pos!=sub_size;++sub_pos){
                sum+=((memory[curr_pos]).memory[sub_pos]).curr_value;
            }
            return sum/sub_size;
        }
        else return (memory[y_idx*x_size+x_idx]).curr_value;
    }

    Grid& operator()(size_t x_idx, size_t y_idx) { return memory[y_idx*x_size+x_idx]; }

    size_t get_xsize() const { return x_size; }
    size_t get_ysize() const { return y_size; }

    Grid& operator=(T val) {
        if(x_size==0){
            curr_value=val;
        }else{
            for(size_t i=0; i!=x_size*y_size; ++i){
                memory[i] = val;
            }
        }
        return *this;
    }

    Grid& get_subgrid(size_t x_idx, size_t y_idx) { return memory[y_idx*x_size+x_idx]; }
    Grid& get_subgrid(size_t x_idx, size_t y_idx) const { return memory[y_idx*x_size+x_idx]; }





    Grid& make_subgrid(size_t x_idx, size_t y_idx, size_t x_sub_idx, size_t y_sub_idx) {
        Grid temp_grid(x_sub_idx, y_sub_idx);
        temp_grid = this(x_idx, y_idx);
        std::swap((memory[y_idx*x_size+x_idx]).curr_value,  temp_grid.curr_value);
        std::swap((memory[y_idx*x_size+x_idx]).x_size, temp_grid.x_size);
        std::swap((memory[y_idx*x_size+x_idx]).y_size, temp_grid.y_size);
        std::swap((memory[y_idx*x_size+x_idx]).memory, temp_grid.memory);
        std::swap((memory[y_idx*x_size+x_idx]).is_sub_grid, temp_grid.is_sub_grid);
        return *this;
    }


    Grid& collapse_subgrid(size_t x_idx, size_t y_idx) {
        Grid temp_grid(0, 0);
        temp_grid = this(x_idx, y_idx);
        std::swap((memory[y_idx*x_size+x_idx]).curr_value,  temp_grid.curr_value);
        std::swap((memory[y_idx*x_size+x_idx]).x_size, temp_grid.x_size);
        std::swap((memory[y_idx*x_size+x_idx]).y_size, temp_grid.y_size);
        std::swap((memory[y_idx*x_size+x_idx]).memory, temp_grid.memory);
        std::swap((memory[y_idx*x_size+x_idx]).is_sub_grid, temp_grid.is_sub_grid);
        return *this;
    }


    friend std::ostream& operator<<(std::ostream &out, const Grid &curr_grid) {
        if(curr_grid.is_sub_grid){
            out<<"\n Grid:\n";
            for(size_t i=0;i!=curr_grid.x_size;++i){
                out<<"\n< ";
                for (size_t j=0 ; j!=curr_grid.y_size; ++j){
                    out<< curr_grid.memory[i*curr_grid.y_size+j] << " ";
                }
                out<<">\n";

            }

        }else{
            out<<curr_grid.curr_value<<" ";
        }
        return out;
    }

    friend std::istream& operator>>(std::istream &in, Grid &curr_grid) {
        if(curr_grid.is_sub_grid){
            for(size_t i=0; i!=curr_grid.x_size*curr_grid.y_size; ++i){
                in>>curr_grid.memory[i];
            }
        }
        else{
            in>>curr_grid.curr_value;
        }
        return in;
    }
};





int main(int argc, const char* argv[]) {
    float new_val;
    std::cin>>new_val;
    Grid<float> gr1(3,2);
    gr1 = new_val;
    Grid<float> gr2(gr1);
    gr2(0,1) = gr2;
    gr2(0,1)(1,0) = 100.0;
    std::cout<<gr2;
    std::cout<<"---";
    gr2(0,1) = 10.0;

    std::cout<<gr2;

    return 0;
}
