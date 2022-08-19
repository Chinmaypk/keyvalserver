#include "KVHeader.h"

// //todo locking
int cache_size;
list<string> cache_queue;
unordered_map<string, list<string>::iterator> key_queue;
unordered_map<string, string> key_value;
extern FILE* file;
int status;


void init_cache(CONFIGRATION * config_file)
{
    cache_size = config_file->cache_size;
    
}

int put(char * message)
{
    char *key = (char*)malloc(sizeof(char)*256);
    char *kvalue = (char*)malloc(sizeof(char)*256);
    memset(key, 0, 256);
    memset(kvalue, 0, 256);
    memcpy(key, &message[1], 256);
    // key[256] = '\0';
    memcpy(kvalue, &message[257], 256);
    // kvalue[256] = '\0';
    string key_ = key;
    string value_ = kvalue;
    
    printf("Message:%s", message);
    printf("PUT:Key is %s\n", key_);
    printf("kvalue is %s\n", value_);
    

    //todo Store first to KV_store
    status =  insert(file, key, kvalue);

    if (status == 1)
    {
        if(key_queue.find(key_) == key_queue.end()){
            cout<<"CACHE:Not in cache"<<"\n";
            if(cache_queue.size() == cache_size){//modified write in KVstore
                string last_elem = cache_queue.back();
                printf("CACHE:Replaced \n");
                key_queue.erase(last_elem);
                key_value.erase(last_elem);
                // cache_map.erase(last_elem);
                cache_queue.pop_back();
            }
            cache_queue.push_front(key_);
            // cache_map[key].value = kvalue;
            // cache_map[key].iter = cache_queue.begin();
            key_value[key_] = value_;
            key_queue[key_] = cache_queue.begin();
            
        }
        else//kvalue update
        {
            cout<<"CACHE: kvalue updated\n";
            cache_queue.erase(key_queue[key_]);
            cache_queue.push_front(key_);
            // cache_map[key].value = kvalue;
            // cache_map[key].iter = cache_queue.begin();
            key_value[key_] = value_;
            key_queue[key_] = cache_queue.begin();
        }
    }

    printf("...............Elemenst in Queue...............\n");
    for (auto ele: key_value)
    {
        cout << "Key: "<<ele.first<<"\nValue:"<<ele.second<<endl;
    }
    // return 1;
    // free(key);
    // free(kvalue);
    return status;
}

char * get(char* message)
{
    char *key = (char*)malloc(sizeof(char)*256);
    memset(key, 0, 256);
    // memset(kvalue, 0, 257);
    memcpy(key, &message[1], 256);
    // key[256] = '\0';
    // printf("GET:Key is %s\n", key);
    string key_ = key;

    if(key_queue.find(key)==key_queue.end()){
        //todo store in cache
        printf("CACHE:Replaced \n");
        if(cache_queue.size() == cache_size){
            string last_elem = cache_queue.back();
            key_queue.erase(last_elem);
            key_value.erase(last_elem);
            // cache_map.erase(last_elem);
            cache_queue.pop_back();
            
        }
        cache_queue.push_front(key_);
            // cache_map[key].value = kvalue;
            // cache_map[key].iter = cache_queue.begin();
        key_value[key_] = search_value(file, key);
        key_queue[key_] = cache_queue.begin();
        return &key_value[key_][0];
    }
    else
    {
        printf("Found in cache\n");
        cache_queue.erase(key_queue[key_]);
        cache_queue.push_front(key_);
        key_queue[key_] = cache_queue.begin();
        return &key_value[key_][0];
    }
    
}

int del(char * message)
{
    char *key = (char*)malloc(sizeof(char)*256);
    int deleted;
    memset(key, 0, 256);
    // memset(kvalue, 0, 257);
    memcpy(key, &message[1], 256);
    // key[256] = '\0';
    // printf("CACHE:DEL:Key is %s\n", key);
    string key_ = key;

    if(key_queue.find(key_) == key_queue.end())
    {   
        status = delete_entry(file, key);
        cout<<"CACHE: Deleted from store :("<<"\n";
    }
    else
    {
        cout<<"CACHE: Deleted from cache :("<<"\n";
        cache_queue.erase(key_queue[key_]);
        key_queue.erase(key_);
        status = delete_entry(file, key);
        printf("...............Elemenst in Queue...............\n");
        for (auto ele: key_value)
        {
            cout << "Key: "<<ele.first<<"\nValue:"<<ele.second<<endl;
        }
    }
        
        // cache_map.erase(key);
        
        //todo delete from store if present or sent error
        // status = delete_entry(file, key);
    // }
    
    
    // free(key);
    return status;
    
}