#include <cstdio>
#include <string>

namespace IMG
{
    class Image
    {
     public:
     int id;
     int w,h;
    };

    Image *Load(std::string path)
    {
     printf("Loaded image: %s\n",path.c_str());
     Image *img=new Image;
     img->id=45;
     img->w=800;
     img->h=600;
     return img;
    }

    void Clear(Image *img)
    {
     printf("Destroyed image\n");
     delete img;
    }
}

namespace MUS
{
    class Music
    {
     public:
     int id;
     int length;
    };

    Music *Load(std::string path)
    {
     printf("Loaded music: %s\n",path.c_str());
     Music *mus=new Music;
     mus->id=78;
     mus->length=1500;
     return mus;
    }

    void Clear(Music *mus)
    {
     printf("Destroyed music\n");
     delete mus;
    }
}

#include "RES_MANAGER/Resource_manager.h"

int main()
{
 RES_MANAGER::Centralized_resource_manager sq;
 sq.Set<IMG::Image>(IMG::Load,IMG::Clear);
 sq.Set<MUS::Music>(MUS::Load,MUS::Clear);

 IMG::Image *unu=sq.Get<IMG::Image>("alabala");
 IMG::Image *doi=sq.Get<IMG::Image>("prrrprr");
 IMG::Image *trei=sq.Get<IMG::Image>("alabala");

 MUS::Music *song=sq.Get<MUS::Music>("music");

 sq.Destroy(unu);
 unu=NULL;
 unu=sq.Get<IMG::Image>("prrrprr");
 sq.Destroy(unu);
 sq.Destroy(doi);

 sq.Destroy(song);

 sq.Clear<IMG::Image>();
 sq.Clear<MUS::Music>();

 printf("Finished!\n");
 return 0;
}
