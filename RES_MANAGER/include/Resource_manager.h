#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Resource_type_counter.h"
#include <unordered_map>
#include <string>

namespace RES_MANAGER
{
    class Base_resource_manager
    {
     public:
     virtual void Clear()=0;
    };

    template<typename Resource_type>
    class Resource_manager : public Base_resource_manager
    {
     private:
     std::unordered_map<std::string,Resource_type*> resources;
     std::unordered_map<std::string,int> times_loaded;
     Resource_type* (*load_function)(std::string path);
     void (*clear_function)(Resource_type*);

     void Load(std::string path)
     {
      resources[path]=(*load_function)(path);
      times_loaded[path]=0;
     }

     public:
     void Set(Resource_type* (*_load_function)(std::string path),void (*_clear_function)(Resource_type*))
     {
      load_function=_load_function;
      clear_function=_clear_function;
      resources.reserve(1024);
      times_loaded.reserve(1024);
     }

     Resource_type *Get(std::string path)
     {
      if(resources.count(path)==0)
         Load(path);
      times_loaded[path]++;
      return resources[path];
     }

     void Destroy(Resource_type *&res)
     {
      auto i=resources.begin();

      while(i!=resources.end() && i->second!=res)
            i++;

      if(i==resources.end())
         return;

      times_loaded[i->first]--;
      if(times_loaded[i->first]==0)
         {
          clear_function(res);
          times_loaded.erase(i->first);
          resources.erase(i->first);
         }
      res=NULL;
     }

     void Clear()
     {
      for(auto &i : resources)
          {
           clear_function(i.second);
          }
      resources.clear();
      times_loaded.clear();
     }

     static inline int Get_type_id()
     {
      static int type_id=Resource_type_counter::type_counter++;
      return type_id;
     }
    };

    template <typename Resource_type>
    static int Get_resource_manager_type_id()
    {
     return Resource_manager<Resource_type>::Get_type_id();
    }
}

#endif // RESOURCE_MANAGER_H
