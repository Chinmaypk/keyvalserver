#include "KVHeader.h"

// //todo locking
int cache_size;
list<string> cache_queue;
unordered_map<string, list<string>::iterator> key_queue;
unordered_map<string, string> key_value;
extern FILE *file;
int status;
char * value_;
string message_str, key, kvalue;

void init_cache(CONFIGRATION *config_file)
{
    cache_size = config_file->cache_size;
}

int put(char *message)
{
    message_str = message;
    key = message_str.substr(1, 256);
    kvalue = message_str.substr(257, 256);

    status = insert(file, &key[0], &kvalue[0]);
    if (status == 1)
    {
        if (key_queue.find(key) == key_queue.end())
        {
            if (cache_queue.size() == cache_size)
            {
                string last_elem = cache_queue.back();
                key_queue.erase(last_elem);
                key_value.erase(last_elem);
                cache_queue.pop_back();
            }
            cache_queue.push_front(key);
            key_value[key] = kvalue;
            key_queue[key] = cache_queue.begin();
        }
        else
        {
            cout << "CACHE: kvalue updated\n";
            cache_queue.erase(key_queue[key]);
            cache_queue.push_front(key);
            key_value[key] = kvalue;
            key_queue[key] = cache_queue.begin();
        }
    }
    return status;
}

char *get(char *message)
{
    message_str = message;
    key = message_str.substr(1, 256);
    
    if (key_queue.find(key) == key_queue.end())
    {
    value_ = search_value(file, &key[0]);
    // return value_;
        if(value_ == NULL){
            return NULL;
        }
        if (cache_queue.size() == cache_size)
        {
            string last_elem = cache_queue.back();
            key_queue.erase(last_elem);
            key_value.erase(last_elem);
            cache_queue.pop_back();
        }
        cache_queue.push_front(key);
        key_value[key] = value_;
        key_queue[key] = cache_queue.begin();
        return &key_value[key][0];
    }
    else
    {
        cache_queue.erase(key_queue[key]);
        cache_queue.push_front(key);
        key_queue[key] = cache_queue.begin();
        return &key_value[key][0];
    }
    
}
int del(char * message)
{
    message_str = message;
    key = message_str.substr(1, 256);
    // return status;
    if(key_queue.find(key) != key_queue.end())
    {   
        // cout<<"CACHE: Deleted from cache :("<<"\n";
        cache_queue.erase(key_queue[key]);
        key_value.erase(key);
        key_queue.erase(key);
        // cache_map.erase(key);
    }
       //todo delete from store if present or sent error
    status = delete_entry(file, &key[0]);
    // free(key);
    return status;
    
}

