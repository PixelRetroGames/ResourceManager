#ifndef CENTRALIZED_RESOURCE_MANAGER
#define CENTRALIZED_RESOURCE_MANAGER

#include "Resource_manager.h"
#include <memory>
#include <map>

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

namespace RES_MANAGER
{
    class Centralized_resource_manager
    {
     private:
     std::map<int,Base_resource_manager*> resource_managers;

     public:
     template<typename Resource_type>
     Resource_type *Get(std::string path)
     {
      Resource_manager<Resource_type> *rm=Get_resource_manager<Resource_type>();
      if(rm!=nullptr)
         return rm->Get(path);
      return nullptr;
     }

     template<typename Resource_type>
     void Destroy(Resource_type *&res)
     {
      Resource_manager<Resource_type> *rm=Get_resource_manager<Resource_type>();
      if(rm!=nullptr)
         rm->Destroy(res);
     }

     template<typename Resource_type>
     void Clear()
     {
      Resource_manager<Resource_type> *rm=Get_resource_manager<Resource_type>();
      if(rm!=nullptr)
         rm->Clear();
     }

     void Clear_all()
     {
      for(int i=0;i<resource_managers.size();i++)
          {
           if(resource_managers[i]!=nullptr)
              resource_managers[i]->Clear();
          }
     }

     template<typename Resource_type>
     void Remove()
     {
      Resource_manager<Resource_type> *rm=Get_resource_manager<Resource_type>();
      if(rm==nullptr)
         return;
      Clear<Resource_type>();
      int type_id=Get_resource_manager_type_id<Resource_type>();
      resource_managers.erase(type_id);
      delete rm;
     }

     void Remove_all()
     {
      Clear_all();
      std::map<int,Base_resource_manager*>().swap(resource_managers);
     }

     template<typename Resource_type>
     void Set(Resource_type* (*load_function)(std::string path),void (*clear_function)(Resource_type*))
     {
      int type_id=Get_resource_manager_type_id<Resource_type>();
      if(!resource_managers.count(type_id))
         {
          resource_managers[type_id]=new Resource_manager<Resource_type>;
          Get_resource_manager<Resource_type>()->Set(load_function,clear_function);
         }
     }

     template <typename Resource_type>
     Resource_manager<Resource_type> *Get_resource_manager()
     {
      int type_id=Get_resource_manager_type_id<Resource_type>();
      if(resource_managers.count(type_id))
         return resource_managers[type_id];
      else
         return nullptr;
     }
    };
}

#endif // CENTRALIZED_RESOURCE_MANAGER
