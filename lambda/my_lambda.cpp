#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

template<typename T>
T to_palindrome(T curr_container) {
    std::vector<std::string> res;
    std::copy_if(curr_container.begin(), curr_container.end(), std::back_inserter(res), [](std::string s) {
        int N = s.size();
        int ind = 0;
        while (ind<N/2){
            if (s[ind]!=s[N-ind-1]){
                return false;
            }
            ind++;
        }
        return true;
    });
    T result(res.begin(),res.end());
    return result;
}

int main() {
    std::vector<std::string> str_vec;
    int counter;
    std::cin>>counter;
    for(int i=0; i<counter; i++) {
        std::string curr;
        std::cin>>curr;
        str_vec.push_back(curr);
    }
    std::vector<std::string> str_pal_vec = to_palindrome(str_vec);
    std::cout << "Number of pal: " << str_pal_vec.size() << std::endl;
    for (int i = 0; i< str_pal_vec.size();i++){
        std::cout << str_pal_vec[i] << " ";
    }
}
