#include <stdio.h>
 
 //�ṹ��洢mp3 ID3V1��Ϣ
 struct mp3
 {
     char header[4];
     char title[31];
     char artist[31];
     char album[31];
     char year[5];
     char comment[31];
     char genre[2];
 };
 
 //Genreת������
 char genre[148][40]= {"Blues","ClassicRock","Country","Dance","Disco","Funk","Grunge","Hip-Hop","Jazz","Metal",
                       "NewAge","Oldies","Other","Pop","R&B","Rap","Reggae","Rock","Techno","Industrial",
                       "Alternative","Ska","DeathMetal","Pranks","Soundtrack","Euro-Techno","Ambient","Trip-Hop",
                       "Vocal","Jazz+Funk","Fusion","Trance","Classical","Instrumental","Acid","House","Game","SoundClip",
                       "Gospel","Noise","AlternRock","Bass","Soul","Punk","Space","Meditative","InstrumentalPop",
                       "InstrumentalRock","Ethnic","Gothic","Darkwave","Techno-Industrial","Electronic","Pop-Folk",
                       "Eurodance","Dream","SouthernRock","Comedy","Cult","Gangsta","Top40","ChristianRap",
                       "Pop/Funk","Jungle","NativeAmerican","Cabaret","NewWave","Psychadelic","Rave","Showtunes",
                       "Trailer","Lo-Fi","Tribal","AcidPunk","AcidJazz","Polka","Retro","Musical","Rock&Roll",
                       "HardRock","Folk","Folk-Rock","NationalFolk","Swing","FastFusion","Bebob","Latin","Revival",
                       "Bluegrass","Avantgarde","GothicRock","ProgessiveRock","PsychedelicRock","SymphonicRock",
                       "SlowRock","BigBand","Chorus","EasyListening","Acoustic","Humour","Speech","Chanson","Opera",
                       "ChamberMusic","Sonata","Symphony","BootyBass","Primus","PornGroove","Satire","SlowJam","Club",
                       "Tango","Samba","Folklore","Ballad","PowerBallad","RhythmicSoul","Freestyle","Duet","PunkRock",
                       "DrumSolo","Acapella","Euro-House","DanceHall","Goa","Drum&Bass","Club-House","Hardcore",
                       "Terror","Indie","BritPop","Negerpunk","PolskPunk","Beat","ChristianGangstaRap","HeavyMetal",
                       "BlackMetal","Crossover","ContemporaryChristian","ChristianRock","Merengue","Salsa","TrashMetal",
                       "Anime","JPop","Synthpop"
                      };
 
 int main(int argc,char* argv[])
 {
     FILE *fp;
     char *file_name = "22.mp3";
     struct mp3 mp3_info;//��Ŷ�ȡ��mp3 ID3V1��Ϣ
 
     fp = fopen(file_name,"r");
     if( fp == NULL)
     {
         printf("can't open file %s",file_name);
         return -1;
     }
 
     //mp3��ĩβ��128�ֽڴ�ţ�ƫ��-128
     fseek(fp,-128,SEEK_END);
 
     //��ȡ��ǩͷ
     fread(mp3_info.header,sizeof(char),3,fp);
 
     //��ǩͷ������"TAG"������Ϊû�б�ǩ
     if(!(mp3_info.header[0] == 'T'
             && mp3_info.header[1] == 'A'
             && mp3_info.header[2] == 'G'))
     {
         printf("%s contains no ID3 v1 tag \r\n",file_name);
     }
     else
     {
         //��ȡ��Ϣ
         fread(mp3_info.title,sizeof(char),30,fp);
         fread(mp3_info.artist,sizeof(char),30,fp);
         fread(mp3_info.album,sizeof(char),30,fp);
         fread(mp3_info.year,sizeof(char),4,fp);
         fread(mp3_info.comment,sizeof(char),30,fp);
         fread(mp3_info.genre,sizeof(char),1,fp);
         mp3_info.title[30] = mp3_info.artist[30] = mp3_info.album[30] = mp3_info.year[4] = mp3_info.comment[30] = mp3_info.genre[1] = 0;
         unsigned char uc = (unsigned char)mp3_info.genre[0];
 
         //��ӡ��Ϣ
         printf("Title : %s\n",mp3_info.title);
         printf("Artist : %s\n",mp3_info.artist);
         printf("Album : %s\n",mp3_info.album);
         printf("Year : %s\n",mp3_info.year);
         printf("Comment : %s\n",mp3_info.comment);
         //�ж�Genre�Ƿ��ںϷ���Χ
         if(uc < 148)
         {
             printf("Genre : %s\n",genre[uc]);
         }
         else
         {
             printf("Genre : Invalid\n");
         }
     }
 
     fclose(fp);
     return 0;
 }