#include <bits/stdc++.h>
using namespace std;
// Defining the blueprint
// so we want the blueprint to be abstarct
// so we will have all the functions as pure virtual functions
struct IndexBackend
{
    virtual void insert(int k, int v) = 0;
    virtual bool find(int k, int &v) = 0;
    virtual vector<pair<int, int>> range(int l, int r) = 0;
    virtual vector<pair<int, int>> dump_all() = 0;
    virtual ~IndexBackend() {}
    // so we have the insert find range and copy queries
    // the destructor has been made virtual so that the
    // we dont face issue running the children's destructors
};
// we made a struct instead of class to avoid wrting
// public again
// HASH TABLE
// its good for searching not great for range based queries

// this is public inheritance so we must satify all the pure
// virtual functions
class HashIndex : public IndexBackend
{
    unordered_map<int, int> table;

public:
    virtual void insert(int k, int v) override
    {
        table[k] = v;
    }
    virtual bool find(int k, int &v) override
    {
        if (table.find(k) == table.end())
        {
            return false;
        }
        else
        {
            v = table[k];
            return true;
        }
    }
    virtual vector<pair<int, int>> range(int l, int r) override
    {
        vector<pair<int, int>> rh; // range holder
        for (auto it : table)
        {
            if (it.first >= l && it.first <= r)
            {
                rh.push_back({it.first, it.second});
            }
        }
        return rh;
    }
    virtual vector<pair<int, int>> dump_all() override
    {
        vector<pair<int, int>> nh; // new holder
        for (auto it : table)
            nh.push_back(it);
        return nh;
    }
};
// now HashIndex is not an absctarct class all pure virtual functions have been made virtual functions
// so we can have objects

// BST
// better for range queries
// we use a map to implement a balanced bst (red black tree)
// map is actually red black tree under the hood
class BSTIndex : public IndexBackend
{
    map<int, int> tree;
    virtual void insert(int k, int v) override
    {
        tree[k] = v;
    };
    virtual bool find(int k, int &v) override
    {
        if (tree.find(k) == tree.end())
        {
            return false;
        }
        else
        {
            v = tree[k];
            return true;
        }
    };
    virtual vector<pair<int, int>> range(int l, int r) override
    {
        vector<pair<int, int>> rh;
        auto it = tree.lower_bound(l);
        while (it != tree.end() && it->first <= r)
        {
            rh.push_back(*it);
            it++;
        }
        return rh;
    };
    virtual vector<pair<int, int>> dump_all() override
    {
        vector<pair<int, int>> data;
        for (auto it : tree)
            data.push_back(it);
        return data;
    }
};
// now we need a way so that we keep past trend in cost but not in a way that is overpowering what is happening now
// so we need a decay mechanism
struct WorkloadStats
{
    double point = 0; // find()
    double range = 0; // range()

    void decay()
    {
        point *= 0.95;
        range *= 0.95;
    }
};

// now comes the final most intresting part the main logic that switches calculates cost basically the brain of the
// program

class AdaptiveHybridIndex
{
    unique_ptr<IndexBackend> backend;
    WorkloadStats stats;
    int size = 0;
    // Switch from one structure to another
    void migrate(unique_ptr<IndexBackend> next)
    {
        cout << "\n[INFO] Switching data structure...\n";
        auto data = backend->dump_all();
        for (auto it : data)
            next->insert(it.first, it.second);
        backend = move(next);
    }
    void maybe_switch()
    {
        double p = stats.point;
        double r = stats.range;
        double n = max(1, size);

        double cost_hash = p * 1 + r * n;
        double cost_bst = p * log2(n) + r * log2(n);

        cout << "[COST] "
             << "Hash=" << cost_hash
             << " | BST=" << cost_bst
             << " | size=" << size << "\n";

        if (dynamic_cast<HashIndex *>(backend.get()))
        {
            if (cost_bst + n < cost_hash)
            {
                cout << "[DECISION] Switching to BST\n";
                migrate(make_unique<BSTIndex>());
            }
        }
    }

public:
    AdaptiveHybridIndex()
    {
        backend = make_unique<HashIndex>();
        cout << "[INIT] Started as HashIndex\n";
    }

    void insert(int key, int value)
    {
        stats.decay();
        backend->insert(key, value);
        size++;
        maybe_switch();
    }

    bool find(int key, int &value)
    {
        stats.decay();
        stats.point++;
        maybe_switch();
        return backend->find(key, value);
    }

    vector<pair<int, int>> range(int l, int r)
    {
        stats.decay();
        stats.range++;
        maybe_switch();
        return backend->range(l, r);
    }
};

// now lets look at the evaluation by taking some exmaples
int main()
{
    AdaptiveHybridIndex ahi;

    cout << "\n[PHASE 1] Insert data\n";
    for (int i = 1; i <= 30; i++)
    {
        ahi.insert(i, i * 10);
    }

    cout << "\n[PHASE 2] Mixed workload (50+ operations)\n";

    srand(time(nullptr));

    int OPERATIONS = 60;
    for (int i = 1; i <= OPERATIONS; i++)
    {
        int choice = rand() % 2;

        if (choice == 0)
        {

            int key = rand() % 30 + 1;
            int value;
            ahi.find(key, value);
            cout << "[OP " << i << "] find(" << key << ")\n";
        }
        else
        {

            int l = rand() % 20 + 1;
            int r = l + (rand() % 8 + 3); // range length 3–10
            ahi.range(l, r);
            cout << "[OP " << i << "] range(" << l << ", " << r << ")\n";
        }
    }

    cout << "\n[END] Program finished\n";
    return 0;
}
