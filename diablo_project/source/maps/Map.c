//map.png
//Converti avec GBA Graphics par Br�nni
//Map ("","map.til.c",0,ffffffff)
//Taille: 8*8 * 60*45
//M�moire: 5400 octets

#include "../ulScreenDisplay.h"
#include "../objects.h"
#include "../interface.h"
#include "map.h"

char currentMap[30];
u16* map_col=NULL;
short *map=NULL;
bool loadmap(char* name,int sizemap,int sizecol);

void changemap(char* mapname)
{
    int i,tilemap_x,tilemap_y;
    char buffer[30];
    char tileset[30];
    FILE* file=NULL;
    sprintf(buffer,"/maps/%s.dat",mapname);
    file=fopen(buffer, "rb");
    if(file==NULL) ERROR("error loading map\n%s",buffer);
    else if( fscanf(file,"[name=<%29[a-z A-Z]>]\n\
                        size.x=%i\n\
                        size.y=%i\n\
                        tileset=<%29[a-z A-Z]>\n\
                        tilemap.x=%i\n\
                        tilemap.y=%i\n\
                        hero.x=%i\n\
                        hero.y=%i\n",mapname,&MAPSIZE_X,&MAPSIZE_Y,tileset,&tilemap_x,&tilemap_y,&hero.x,&hero.y))
    {
        hero.x=norm_fix(hero.x);
        hero.y=norm_fix(hero.y);

        ulDeleteImage(mapTiles);
        ulDeleteMap(Mymap);
        for (i=0; i<MAX_OBJECT; i++)
        {
            deleteObject(i);
        }
        for (i=0; i<MAX_BGOBJECT; i++)
        {
            deleteBgObject(i);
        }
        curMaxObject=0;
        curMaxBgObject=0;

        if(!loadmap(mapname,tilemap_x*tilemap_y,MAPSIZE_X*MAPSIZE_Y))
            while(1)
            {
                ulStartDrawing2D();
                ulDrawTextBox(3,171,253,190,"Error loading map",0);
                ulEndDrawing();
                WaitForVBL();
            }
        sprintf(buffer,"/gfx/%s_png.png",tileset);
        mapTiles = ulLoadImageFilePNG(buffer,0, UL_IN_VRAM, UL_PF_PAL4);
        if(map!=NULL && map_col!=NULL)Mymap = ulCreateMap(mapTiles,/*Tileset*/map,8,8,/*Tiles size*/tilemap_x,tilemap_y,/*Map size*/UL_MF_U16);//Map format
        else while(1)ERROR("Can't load map.");//error
        spawnObjects();
        sprintf(currentMap,"%s",mapname);

    }
    else topPrintf(130,40,"Error loading map");


    for (i=1; i<MAX_DATASPRITES; i++)
    {
        ulUnrealizeImage(spritedatabase[i].image);
    }
}


bool loadmap(char* name,int sizemap,int sizecol)
{
    FILE* file=NULL;
    char buffer[30];

    sprintf(buffer,"/maps/%s.map.bin",name);
    file=fopen(buffer, "rb");
    if(map!=NULL)free(map);
    map=malloc(sizemap*sizeof(short));
    if(map!=NULL && file!=NULL)fread(map,sizeof(short), sizemap, file);
    else return 0;
    fclose(file);

    sprintf(buffer,"/maps/%s.col.bin",name);
    if (map_col!=NULL)free(map_col);
    map_col=malloc(sizecol*sizeof(short));
    file=fopen(buffer, "rb");
    if(map_col!=NULL && file!=NULL)fread(map_col,sizeof(short), sizecol, file);
    else return 0;
    fclose(file);

    return 1;
}





