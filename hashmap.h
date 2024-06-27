#ifndef HashMap_h
#define HashMap_h

#include <string>
#include <vector>
#include <list>

template <typename T>
struct Node {
    std::string key;
    T value;
    
    // allow way to initialize Node
    Node(const std::string& key, const T& value)
    : key(key), value(value) {}
};

template <typename T>
class HashMap
{
    public:
        // initialize private members
        HashMap(double max_load = 0.75) : m_max_load(max_load), buckets(10), m_count(0) {}
        ~HashMap(){} // destructor; deletes all of the items in the hashmap
        int size() const{return m_count;} // return the number of associations in the hashmap
         // The insert method associates one item (key) with another (value).
         // If no association currently exists with that key, this method inserts
         // a new association into the hashmap with that key/value pair. If there is
         // already an association with that key in the hashmap, then the item
         // associated with that key is replaced by the second parameter (value).
         // Thus, the hashmap must contain no duplicate keys.
        void insert(const std::string& key, const T& value);
         // Defines the bracket operator for HashMap, so you can use your map like this:
         // your_map["david"] = 2.99;
         // If the key does not exist in the hashmap, this will create a new entry in
         // the hashmap and map it to the default value of type T. Then it will return a
         // reference to the newly created value in the map.
        T& operator[](const std::string& key);
         // If no association exists with the given key, return nullptr; otherwise,
         // return a pointer to the value associated with that key. This pointer can be
         // used to examine that value within the map.
        const T* find(const std::string& key) const;
         // If no association exists with the given key, return nullptr; otherwise,
         // return a pointer to the value associated with that key. This pointer can be
         // used to examine that value or modify it directly within the map.
        T* find(const std::string& key) {
            const auto& hm = *this;
            return const_cast<T*>(hm.find(key));
        }
    private:
        std::vector<std::list<Node<T>>> buckets;
        double m_max_load;
        int m_count;
        
        // returns hashed key
        size_t getBucketIndex(const std::string& key) const {
            static std::hash<std::string> hash;
            return hash(key) % buckets.size();
        }
        
        // checks if load exceeded or reached
        void checkLoad() {
            if(size()/buckets.size() >= m_max_load){resize();}
        }
        
        // method to rehash by creating new vector with double size
        void resize() {
            std::vector<std::list<Node<T>>> old_buckets(buckets.size()*2);
            buckets.swap(old_buckets);
            for(const auto& bucket : old_buckets){
                for(const auto& kv : bucket){
                    size_t new_index = getBucketIndex(kv.key);
                    buckets[new_index].emplace_back(kv);
                }
            }
        }
};

template <typename T>
void HashMap<T>::insert(const std::string& key, const T& value){
    checkLoad();
    auto& bucket = buckets[getBucketIndex(key)];
    auto it = find(key);
    if(it != nullptr){*it = value;} // replace key's value if key found
    else{
        bucket.push_back(Node(key, value)); // otherwise add new node
        ++m_count;
    }
}

template <typename T>
T& HashMap<T>::operator[](const std::string& key) {
    checkLoad();
    
    auto& bucket = buckets[getBucketIndex(key)];
    auto it = find(key);
    
    if(it != nullptr){return *it;} // return reference to value
    
    else{
        insert(key, T());
        return bucket.back().value; // return newly constructed value
    }
}

template <typename T>
const T* HashMap<T>::find(const std::string& key) const {
    auto bucketIndex = getBucketIndex(key);
    for (auto& node : buckets[bucketIndex]) {
        if(node.key == key){return &node.value;}
    }
    return nullptr;
}

#endif
