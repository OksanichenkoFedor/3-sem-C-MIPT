#include <iostream>
#include <set>
#include <random>
#include<vector>
#include<chrono>
#include<iostream>
#include<algorithm>
#include<cassert>
#include<fstream>
#include<string>
#include <algorithm>

class AbstractState
{
public:
    virtual bool contains(int s) const = 0;
};

class DiscreteState : public AbstractState{
private:
    int const state;

public:
    DiscreteState(int state): state(state) { }

    bool contains(int s) const override
    {
        return s == state;
    }
};

class SegmentState : public AbstractState {
private:
    int const beg, end;

public:
    SegmentState(): beg(0), end(-1) { }
    SegmentState(int beg, int end): beg(beg), end(end) { }

    bool contains(int s) const override
    {
        return s >= beg && s <= end;
    }
};

class SetState : public AbstractState {
protected:
    std::set<int> const states;

public:

    SetState(): states() { }
    SetState(std::set<int> const &src): states(src) { }

    bool contains(int s) const override
    {
        return states.count(s) > 0;
    }
    std::set<int> getSet() const
    {
        return states;
    }
};

class SegmentPlusState : public AbstractState {
private:
    const SegmentState ss;
    const SetState s;


public:
    SegmentPlusState(): s(), ss(-1,0) { }
    SegmentPlusState(int beg, int end, std::set<int> const &src): s(src), ss(beg,end) { }

    bool contains(int num) const override
    {
        return ss.contains(num) || s.contains(num);
    }
};

class SegmentMinusState : public AbstractState {
private:
    const SegmentState ss;
    const SetState s;

public:
    SegmentMinusState(): s(), ss(-1,0) { }
    SegmentMinusState(int beg, int end, std::set<int> const &src): s(src), ss(beg,end) { }

    bool contains(int num) const override
    {
        return ss.contains(num) && !s.contains(num);
    }
};

class SetUnificationState : public SetState {
private:
    std::set<int> const states;
    std::set<int> UnifiSets(const SetState& s1, const SetState& s2)
    {
        std::set<int> new_set = (s1.getSet());
        std::set<int> new_set_1 = s2.getSet();
        new_set.insert(new_set_1.begin(), new_set_1.end());
        return new_set;
    }

public:

    SetUnificationState(): SetState() { }
    SetUnificationState(SetState &s1, SetState &s2): SetState(UnifiSets(s1,s2)) { }

};

class ProbabilityTest {
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;

public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count): seed(seed), test_min(test_min),test_max(test_max), test_count(test_count) { }

    float operator()(AbstractState const &s) const {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min,test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good)/static_cast<float>(test_count);
    }
};

double CheckProbability(const AbstractState& s, int seed, int test_min, int test_max, unsigned test_count)
{
    ProbabilityTest pt(seed,test_min,test_max,test_count);
    return pt(s);
}

int main(int argc, const char * argv[]) {
    std::ifstream fin("test_vhod.txt");
    std::ofstream fout_s("2SegmentS.txt");
    std::ofstream fout_ss("2SetS.txt");
    std::ofstream fout_d("2DiscretS.txt");
    DiscreteState d(1);
    SegmentState s(0,10);
    SetState ss({1, 3, 5, 7, 23, 48, 57, 60, 90, 99});
    ProbabilityTest pt(10,0,100,100000);
    //std::cout << pt(d) << std::endl;
    SegmentMinusState sm(0,10, {1,3,5,7,9,11});
    SegmentPlusState sp(0,10, {1,12,14,16});
    int max = 0;
    while(fin >> max)
    {
        //fout_s << (10.0/max) << " " << CheckProbability(s,10,0,max,100000) << "\n";
        //fout_ss << (10.0/max) << " " << CheckProbability(ss,191,0,max,100000) << "\n";
        fout_d << (1.0/(max+1.0)) << " " << CheckProbability(d,100,0,max,100000) << "\n";
    }
    return 0;
}
