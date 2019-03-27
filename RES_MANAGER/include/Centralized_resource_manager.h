#ifndef CENTRALIZED_RESOURCE_MANAGER
#define CENTRALIZED_RESOURCE_MANAGER

#include "Resource_manager.h"
#include <memory>
#include <vector>

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

namespace RES_MANAGER
{
    class Centralized_resource_manager
    {
     private:
     std::vector<std::unique_ptr<Base_resource_manager>> resource_managers;

     public:
     template<typename Resource_type>
     Resource_type *Get(std::string path)
     {
      return Get_resource_manager<Resource_type>()->Get(path);
     }

     template<typename Resource_type>
     void Destroy(Resource_type *&res)
     {
      Get_resource_manager<Resource_type>()->Destroy(res);
     }

     template<typename Resource_type>
     void Clear()
     {
      Get_resource_manager<Resource_type>()->Clear();
     }

     template<typename Resource_type>
     void Set(Resource_type* (*load_function)(std::string path),void (*clear_function)(Resource_type*))
     {
      int type_id=Get_resource_manager_type_id<Resource_type>();
      if(type_id>=resource_managers.size())
         resource_managers.resize(type_id+1);
      if(resource_managers[type_id]==0)
         {
          resource_managers[type_id]=make_unique<Resource_manager<Resource_type>>();
          Get_resource_manager<Resource_type>()->Set(load_function,clear_function);
         }
     }

     template <typename Resource_type>
     Resource_manager<Resource_type> *Get_resource_manager()
     {
      int type_id=Get_resource_manager_type_id<Resource_type>();
      return static_cast<Resource_manager<Resource_type>*>(resource_managers[type_id].get());
     }
    };
}

#endif // CENTRALIZED_RESOURCE_MANAGER
