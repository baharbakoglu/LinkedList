#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>

typedef struct _list/*define linked list struct*/
{
    struct _list *next; /*refers to the next element*/
    int nextpointer;
    int datapointer;
    int ownpointer;
    int parentpointer;
    char name[16];
    int attributes;
    int userID;
    int groupID;
    int size;
    int check;
} list;


typedef struct
{
    list *firstelement; /* linked list represents.only The first element is the sign.*/
} linkedlist;


typedef struct {
    char name[8];
}element;

linkedlist *producelinkedlist();
void list_add(linkedlist* bl,int ownpointer,int parentpointer,char name[16],int attributes,int userID,int groupID,int datapointer,int nextpointer,int size);/*adding element in desired linkedlist*/
void read_fat_file(int *arr, int i);
void read_harddisk_file(element *harddiskarr, int a);
void read_dir_file(linkedlist * dirfile_list);
void find_sort(linkedlist *bl, int *lastarr);
void find_last_fat_file(int *arr, int *arrelement, int *lastarr);
void print_harddisk_file(element *harddiskarr, int *lastarr, int a);
void print_fat_file(int *arr, int *lastarr,int a);
void print_dir_file(linkedlist *bl);
int find_size(linkedlist *bl);


int main()
{
    int fatarr[50],lastfatarr[50], fatarrelement[10],i;
    element harddiskarr[50];
    linkedlist * dirfile_list = producelinkedlist();
    read_dir_file(dirfile_list);
    i=find_size(dirfile_list);
    read_fat_file(fatarr,i);
    read_harddisk_file(harddiskarr,i);
    find_sort(dirfile_list,fatarrelement);
    find_last_fat_file(fatarr,fatarrelement,lastfatarr);
    print_harddisk_file(harddiskarr,lastfatarr,i);
    print_fat_file(fatarr, lastfatarr, i);
    print_dir_file(dirfile_list);

    return 0;
}

void read_dir_file(linkedlist * dirfile_list)
{
    FILE *dirfile;
    int i=0;
    char buffer[16];
    unsigned char buff, buff1;
    unsigned int ownpointer,parentpointer,attributes,userID, groupID, datapointer, nextpointer, size;
    if ((dirfile = fopen( "dir.txt", "rb" ) ) == NULL )/*open dir file*/
        printf( "File could not be opened\n" );
    else
    {
        while(i>=0){
                fseek(dirfile,(i+4),SEEK_SET);
                fread(&buffer,1,16,dirfile);
                if(buffer[0]!='\0'){/*controlled name isn't null*/
                    fseek(dirfile,i,SEEK_SET);
                    fread(&buff,1,1,dirfile);
                    fseek(dirfile,(i+1),SEEK_SET);
                    fread(&buff1,1,1,dirfile);
                    ownpointer = ((int)buff *256) + (int)buff1;
                    fseek(dirfile,(i+2),SEEK_SET);
                    fread(&buff,1,1,dirfile);
                    fseek(dirfile,(i+3),SEEK_SET);
                    fread(&buff1,1,1,dirfile);
                    parentpointer = ((int)buff*256) + (int)buff1;
                    fseek(dirfile,(i+20),SEEK_SET);
                    fread(&buff,1,1,dirfile);
                    fseek(dirfile,(i+21),SEEK_SET);
                    fread(&buff1,1,1,dirfile);
                    attributes = ((int)buff*256) + (int)buff1;
                    fseek(dirfile,(i+22),SEEK_SET);
                    fread(&buff,1,1,dirfile);
                    fseek(dirfile,(i+23),SEEK_SET);
                    fread(&buff1,1,1,dirfile);
                    userID = ((int)buff*256) + (int)buff1;
                    fseek(dirfile,(i+24),SEEK_SET);
                    fread(&buff,1,1,dirfile);
                    fseek(dirfile,(i+25),SEEK_SET);
                    fread(&buff1,1,1,dirfile);
                    groupID = ((int)buff*256) + (int)buff1;
                    fseek(dirfile,(i+26),SEEK_SET);
                    fread(&buff,1,1,dirfile);
                    fseek(dirfile,(i+27),SEEK_SET);
                    fread(&buff1,1,1,dirfile);
                    datapointer = ((int)buff*256) + (int)buff1;
                    fseek(dirfile,(i+28),SEEK_SET);
                    fread(&buff,1,1,dirfile);
                    fseek(dirfile,(i+29),SEEK_SET);
                    fread(&buff1,1,1,dirfile);
                    nextpointer = ((int)buff*256) + (int)buff1;
                    fseek(dirfile,(i+30),SEEK_SET);
                    fread(&buff,1,1,dirfile);
                    fseek(dirfile,(i+31),SEEK_SET);
                    fread(&buff1,1,1,dirfile);
                    size = ((int)buff*256) + (int)buff1;
                    list_add(dirfile_list,ownpointer,parentpointer,buffer,attributes,userID, groupID, datapointer, nextpointer, size);/*added data in linked list*/
                    i=i+32;
                }
                else
                    break;
        }

    }
    fclose(dirfile);/*close dir file*/
}
void read_fat_file(int *arr , int i)
{
    FILE *fatfile;
    unsigned short int buff;
    int j=0,a=7;
    if ((fatfile = fopen( "fat.txt", "rb" ) ) == NULL )/*open fat file*/
        printf( "File could not be opened\n" );
    else
    {
        while(i>0){
            fseek(fatfile,a,SEEK_SET);
            fread(&buff,1,1,fatfile);/*read fat data*/
            arr[j]=buff;
            a+=8;
            j++;
            i--;
        }
    }
    fclose(fatfile);/*close fat file*/
}
void read_harddisk_file(element *harddiskarr, int a)
{
    FILE *harddiskfile;
    int i=0,j=0;
    char harddiskdata[8];
    if ((harddiskfile = fopen( "harddisk.txt", "rb" ) ) == NULL )/*open harddisk file*/
        printf( "File could not be opened\n" );
    else
    {
        while(a>0){
            fseek(harddiskfile,i,SEEK_SET);
            fread(&harddiskdata,1,8,harddiskfile);/*read name*/
            strcpy(harddiskarr[j].name,harddiskdata);
            i+=8;
            j++;
            a--;
        }
    }
    fclose(harddiskfile);/*close harddisk file*/

}
linkedlist *producelinkedlist()
{
     linkedlist* bl = (linkedlist*)malloc(sizeof(linkedlist)); /* a new linked list draws up in memory, we're taking about the address.*/
     bl->firstelement = NULL;
     return bl;
}

void list_add(linkedlist* bl,int ownpointer,int parentpointer,char name[16],int attributes,int userID,int groupID,int datapointer,int nextpointer,int size)
{
     list *r = (list*)malloc(sizeof(list)); /* we construct a new element.*/
     r->next = NULL;
     r->ownpointer=ownpointer;/*add values in linkedlist*/
     r->parentpointer=parentpointer;
     strcpy(r->name,name);
     r->attributes=attributes;
     r->userID=userID;
     r->groupID=groupID;
     r->datapointer=datapointer;
     r->nextpointer=nextpointer;
     r->size=size;
     r->check=0;
     list *p = NULL; /* linked list's last element*/
     p = (list *)bl->firstelement; /* Let the starting element from the linked list.*/
     if (p==NULL) /*our list is empty?*/
     {
        bl->firstelement = r;
        return;
    }
    while(p!=NULL) /*try to access the last element.*/
    {
        if (p->next==NULL)
            break;
        p = (list*)p->next; /*jump between nodes*/
    }
    p->next = r; /* We're writing our own address pointer of the last node.*/
}
void find_sort(linkedlist *bl, int *lastarr)
{
    int i=0,cout=0;
    int datapoint=0;
    list *r=NULL;
    r=(list *)bl->firstelement;
    while(r!=NULL){
        if(r->check==0){
            cout++;/*number of how many file we have*/
        }
        r=r->next;
    }
    r=(list *)bl->firstelement;
    while(cout>0)
    {
        if(r->check==0){/*this file controlled or not*/
            cout--;
            if(r->nextpointer==0)/*if this file doesn't have next*/
            {
                r->check=1;
                if(r->size!=0){/*if thhis file is txt file*/
                    lastarr[i]=r->datapointer;
                    r->datapointer=datapoint;
                    datapoint+=r->size;/*change files data pointer*/
                    i++;
                }
                r=(list *)bl->firstelement;/*go to top of list*/
                continue;
            }
            else {
                r->check=1;
                if(r->size!=0){/*if thhis file is txt file*/
                    lastarr[i]=r->datapointer;
                    r->datapointer=datapoint;
                    datapoint+=r->size;/*change files data pointer*/
                    i++;
                }
                int k=r->nextpointer;
                r=(list *)bl->firstelement;
                while(r->ownpointer!=k)/*go to next file*/
                {
                    r=r->next;
                }
                continue;
            }
        }
        else{
            r=r->next;
        }
    }
}
void find_last_fat_file(int *arr, int *arrelement, int *lastarr)/*find fat file's last sorting*/
{
    int i,j=0;
    for(i=0;i<4;i++){
        lastarr[j]=arrelement[i];
        j++;
        while(arr[arrelement[i]]!=255){
            lastarr[j]=arr[arrelement[i]];
            j++;
            arrelement[i]=arr[arrelement[i]];
        }
    }
}
void print_fat_file(int *arr, int *lastarr , int a)
{
    FILE *fatfile;
    int i;
    unsigned short int j=0, k=255;
    for(i=0;i<a;i++)
    {
        if(arr[lastarr[i]]!=255)/*if element doesn't equals to FF*/
        {
            lastarr[i]=i;
        }
        else
        {
            lastarr[i]=255;
        }
    }
    if ((fatfile = fopen( "fat.txt", "wb" ) ) == NULL )/*open fat file to write*/
        printf( "File could not be opened\n" );
    else
    {
        for(i=0;i<a;i++){
            if(lastarr[i]!=255){/*if element doesn't equals to FF*/
                fwrite(&j,1,1,fatfile);
                fwrite(&j,1,1,fatfile);
                fwrite(&j,1,1,fatfile);
                fwrite(&j,1,1,fatfile);
                fwrite(&j,1,1,fatfile);
                fwrite(&j,1,1,fatfile);
                fwrite(&j,1,1,fatfile);
                fwrite(&lastarr[i],1,1,fatfile);
            }
            else{/*if element equals to FF*/
                fwrite(&k,1,1,fatfile);
                fwrite(&k,1,1,fatfile);
                fwrite(&k,1,1,fatfile);
                fwrite(&k,1,1,fatfile);
                fwrite(&k,1,1,fatfile);
                fwrite(&k,1,1,fatfile);
                fwrite(&k,1,1,fatfile);
                fwrite(&lastarr[i],1,1,fatfile);
            }
        }
    }
    fclose(fatfile);/*close fat file*/
}
void print_harddisk_file(element *harddiskarr, int *lastarr, int a)
{
    FILE *harddiskfile;
    int i;
    if ((harddiskfile = fopen( "harddisk.txt", "wb" ) ) == NULL )/*open harddisk file to write*/
        printf( "File could not be opened\n" );
    else
    {
        for(i=0;i<a;i++){
                fwrite(&harddiskarr[lastarr[i]].name,1,8,harddiskfile);/*write new sorting in harddisk file*/
        }
    }
    fclose(harddiskfile);/*close harddisk file*/
}
void print_dir_file(linkedlist *bl)
{
    FILE *dirfile;
    int buff, buff1;
    list *r = NULL;
    r=(list *)bl->firstelement;
    if ((dirfile = fopen( "dir.txt", "wb" ) ) == NULL )/*open dir file to write*/
       printf( "File could not be opened\n" );
    else
    {
        while(r!=NULL){
                    buff1=(r->ownpointer)%256; /*for print own pointer*/
                    buff=(r->ownpointer-buff1)/256;
                    fwrite(&buff,1,1,dirfile);
                    fwrite(&buff1,1,1,dirfile);
                    buff1=(r->parentpointer)%256; /*for printf parent pointer */
                    buff=(r->parentpointer-buff1)/256;
                    fwrite(&buff,1,1,dirfile);
                    fwrite(&buff1,1,1,dirfile);
                    fwrite(&r->name,1,16,dirfile);
                    buff1=(r->attributes)%256; /*for print attributes*/
                    buff=(r->attributes-buff1)/256;
                    fwrite(&buff,1,1,dirfile);
                    fwrite(&buff1,1,1,dirfile);
                    buff1=(r->userID)%256; /*for print user ID*/
                    buff=(r->userID-buff1)/256;
                    fwrite(&buff,1,1,dirfile);
                    fwrite(&buff1,1,1,dirfile);
                    buff1=(r->groupID)%256; /*for print group ID*/
                    buff=(r->groupID-buff1)/256;
                    fwrite(&buff,1,1,dirfile);
                    fwrite(&buff1,1,1,dirfile);
                    buff1=(r->datapointer)%256; /*for print data pointer*/
                    buff=(r->datapointer-buff1)/256;
                    fwrite(&buff,1,1,dirfile);
                    fwrite(&buff1,1,1,dirfile);
                    buff1=(r->nextpointer)%256; /*for print next pointer*/
                    buff=(r->nextpointer-buff1)/256;
                    fwrite(&buff,1,1,dirfile);
                    fwrite(&buff1,1,1,dirfile);
                    buff1=(r->size)%256; /*for print size*/
                    buff=(r->size-buff1)/256;
                    fwrite(&buff,1,1,dirfile);
                    fwrite(&buff1,1,1,dirfile);
                    r = (list*) r->next; /*r'yi ilerlet*/
                }
       }
       fclose(dirfile);/*close dir file*/
}
int find_size(linkedlist *bl)/*find fat and harddisk file's element number*/
{
    int i=0;
    list *r = NULL;
    r=(list *)bl->firstelement;
    while(r!=NULL)
    {
        i+=r->size;
        r = (list*) r->next;
    }
    return i;
}
