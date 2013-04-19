unit Here3DS;
//Unit di supporto per il caricamento di files 3DS
//by HereBit 1999 (realizzata in C++ e portata in Delphi3)

interface
type charp=^char;
const
 //>------ Primary chunk
 MAIN3DS=$4D4D;
 //>------ Main Chunks
 VERSION3DS=$0002;
 EDIT3DS=$3D3D;  // this is the start of the editor config
 KEYF3DS=$B000;  // this is the start of the keyframer config
 //>------ sub defines of EDIT3DS
 EDIT_MATERIAL=$AFFF;
 //#define EDIT_CONFIG1  0x0100
 //#define EDIT_CONFIG2  0x3E3D
 //#define EDIT_VIEW_P1  0x7012
 //#define EDIT_VIEW_P2  0x7011
 //#define EDIT_VIEW_P3  0x7020
 //#define EDIT_VIEW1    0x7001
 //#define EDIT_BACKGR   0x1200
 //#define EDIT_AMBIENT  0x2100
 EDIT_OBJECT=$4000;
 MAT_NAME=$A000;
 MAT_MAP1=$A200;
 MAT_MAP2=$A34C;
 MAP_FILE=$A300;
 MAP_REFLECT=$A220;
 (*
 #define EDIT_UNKNW01  0x1100
 #define EDIT_UNKNW02  0x1201
 #define EDIT_UNKNW03  0x1300
 #define EDIT_UNKNW04  0x1400
 #define EDIT_UNKNW05  0x1420
 #define EDIT_UNKNW06  0x1450
 #define EDIT_UNKNW07  0x1500
 #define EDIT_UNKNW08  0x2200
 #define EDIT_UNKNW09  0x2201
 #define EDIT_UNKNW10  0x2210
 #define EDIT_UNKNW11  0x2300
 #define EDIT_UNKNW12  0x2302
 #define EDIT_UNKNW13  0x3000
 #define EDIT_UNKNW14  0xAFFF
 *)
 //>------ sub defines of EDIT_OBJECT
 OBJ_TRIMESH=$4100;
 OBJ_LIGHT=$4600;
 OBJ_CAMERA=$4700;
 //#define OBJ_UNKNWN01  0x4010
 //#define OBJ_UNKNWN02  0x4012 //>>---- Could be shadow
 //>------ sub defines of OBJ_CAMERA
 //#define CAM_UNKNWN01  0x4710
 //#define CAM_UNKNWN02  0x4720
 //>------ sub defines of OBJ_LIGHT
 //#define LIT_OFF       0x4620
 //#define LIT_SPOT      0x4610
 //#define LIT_UNKNWN01  0x465A
 //>------ sub defines of OBJ_TRIMESH
 TRI_VERTEXL=$4110;
 //#define TRI_FACEL2    0x4111  questo mi sa che è cannato
 TRI_FACEL1=$4120;
 TRI_MATERIAL=$4130;
 TRI_MAPCOORD=$4140;
 TRI_SMOOTH=$4150;
 TRI_LOCAL=$4160;
 TRI_VISIBLE=$4165;
 //>>------ sub defs of KEYF3DS
 KEYF_MESH=$B002;
 KEYF_CAMERA=$B003;
 KEYF_TARGET=$B004;
 //#define KEYF_UNKNWN01 0xB009
 //#define KEYF_UNKNWN02 0xB00A
 KEYF_FRAMES=$B008;
 KEYF_OBJNAME=$B010;
 KEYF_PIVOT=$B013;
 KEYF_POSTRACK=$B020;
 KEYF_ROTTRACK=$B021;
 KEYF_ROLLTRACK=$B024;
 //>>------  these define the different color chunk types
 //#define COL_RGB  0x0010
 //#define COL_TRU  0x0011
 //#define COL_UNK  0x0013
// costanti usate nella riconosci oggetto
 NORMALOBJ=0;
 CAMERA=1;
 LIGHT=2;

 function alloc_3DS(filename:string):charp;
 function get_chunk_type(file3DS:charp; offset:integer):word;
 function get_chunk_fullsize(file3DS:charp; offset:integer):integer;
 function get_chunk_size(file3DS:charp; offset:integer):integer;
 function find_next_chunk(file3DS:charp; offset:integer):integer;
 function find_next_chunk_oftype(file3DS:charp; offset:integer; cktype:word):integer;
 function find_first_subchunk(file3DS:charp; offset:integer):integer;
 function find_first_subchunk_oftype(file3DS:charp; offset:integer; cktype:word):integer;
 function riconosci_oggetto(file3DS:charp; offset:integer):integer;

implementation
uses dialogs,sysutils;

function get_chunk_type(file3DS:charp; offset:integer):word;
var p:^word;
begin
  p:=ptr(integer(file3DS)+offset);
  result:=p^;
end;

function alloc_3DS(filename:string):charp;
var bfile:file of char;
    n:char;
    lung,i:integer;
    file3DS,temp:charp;
begin
  assignfile(bfile,filename);
  reset(bfile);
  lung:=filesize(bfile);
  getmem(file3DS,lung+4);
  for i:=0 to lung-1 do
      begin
        read(bfile,n);
        temp:=ptr(integer(file3DS)+i);
        temp^:=n;
      end;
  n:=chr(0);
  temp:=ptr(integer(file3DS)+lung+1);
  temp^:=n;
  temp:=ptr(integer(file3DS)+lung+2);
  temp^:=n;
  closefile(bfile);
  result:=file3DS;
end;

function get_chunk_fullsize(file3DS:charp; offset:integer):integer;
var cksize:^integer;
begin
  cksize:=ptr(integer(file3DS)+offset+2);
  result:=cksize^;
end;

function get_chunk_size(file3DS:charp; offset:integer):integer;
// restituisce la dimensione del chunk, esclusi i 6 byte iniziali e i subchunks
// purtroppo alcuni chunks hanno dimensione variabile, per questo servono
// i valori di *file3DS e offset ...
// ATTENZIONE: per ora ho fatto solo quelli che mi pare che ci servono!!
begin
  case get_chunk_type(file3DS,offset) of
   MAIN3DS : result:=0;
   VERSION3DS: result:=4;
   EDIT3DS : result:=0;
   {EDIT_OBJECT : { // VARIABILE!!!
                          // è costrituito da una stringa zero terminated.
                          unsigned char *tempos;
                          tempos=(unsigned char *) (file3DS+offset+6);
                          while (*tempos != 0) tempos++;
                          return (int)(tempos)-((int)(file3DS)+offset+6)+1; // ecco la dimensione!
                        }
   OBJ_TRIMESH : result:=0;
   {TRI_VERTEXL : { // VARIABILE!!!
                          unsigned short int *vxnum;
                          vxnum=(unsigned short int *)(file3DS+offset+6);
                          return 3*4*(*vxnum)+2; // ecco la dimensione!
                        }
   {TRI_FACEL1 : { // VARIABILE!!!
                         unsigned short int *fcnum;
                         fcnum=(unsigned short int *)(file3DS+offset+6);
                         return 2*4*(*fcnum)+2; // ecco la dimensione!
                        }
   EDIT_MATERIAL : result:=0;
   {MAT_NAME : { // VARIABILE!!!
                       // è costrituito da una stringa zero terminated.
                          unsigned char *tempos;
                          tempos=(unsigned char *)(file3DS+offset+6);
                          while (*tempos != 0) tempos++;
                          return int(tempos)-((int)(file3DS)+offset+6)+1; // ecco la dimensione!
                     }
   MAT_MAP1 : result:=0;
   MAT_MAP2 : result:=0;
   MAP_REFLECT : result:=0;
   KEYF3DS : result:=0;
   KEYF_MESH : result:=0;
   KEYF_CAMERA : result:=0;
   KEYF_TARGET : result:=0;
  else
    showmessage('Errore: ChunkID '+inttostr(get_chunk_type(file3DS,offset))+
                ' not implemented in get_chunk_size(...)');
    result:=-1;
  end;
end;

function find_next_chunk(file3DS:charp; offset:integer):integer;
// restituisce l'offset del prossimo chunk di stesso livello o inferiore
// a partire da offset dove offset punta a un chunk-id
// (salta i subchunks)
// ATTENZIONE: non controlla se si va oltre la fine del file!!!
begin
  result:=offset+get_chunk_fullsize(file3DS,offset);
end;


function find_next_chunk_oftype(file3DS:charp; offset:integer; cktype:word):integer;
// restituisce l'offset del prossimo chunk a partire da offset del tipo cktype
// dove offset punta a un chunk-id
// (salta i subchunks) e restituisce -1 se non trovato
var myofs:integer;
    mytype:word;
begin
  myofs:=offset;

  repeat
    myofs:=find_next_chunk(file3DS,myofs);
    mytype:=get_chunk_type(file3DS,myofs);
  until (mytype=cktype) or (mytype=0);

  if (mytype=0) then result:=-1
     else result:=myofs;
end;

function find_first_subchunk(file3DS:charp; offset:integer):integer;
// restituisce la posizione del primo subchunk del chunk offset
// dove offset punta a un chunk-id
// se il chunk non ha subchunks ritorna -1
var ckfullsize,cksize:integer;
begin
  ckfullsize:=get_chunk_fullsize(file3DS, offset); // dim con subchunks
  cksize:=get_chunk_size(file3DS, offset)+6;  // dim senza subchunks
  if (ckfullsize=cksize)or (cksize=-1) then result:=-1 // non ha subchunks!
  else result:=offset+cksize; // subchunk trovato
end;


function find_first_subchunk_oftype(file3DS:charp; offset:integer; cktype:word):integer;
// restituisce la posizione del primo subchunk del chunk offset
// dove offset punta a un chunk-id, del tipo cktype
// se il chunk non ha subchunks o quel tipo non viene trovato ritorna -1
var myofs:integer;
begin
  myofs:=find_first_subchunk(file3DS,offset);
  if (myofs=-1) then result:=-1 // non ha subchunks
  else
   begin
    if (get_chunk_type(file3DS,myofs)<>cktype)
       then myofs:=find_next_chunk_oftype(file3DS,myofs,cktype);
    result:=myofs;
   end;
end;

function riconosci_oggetto (file3DS:charp; offset:integer):integer;
// riconosce il tipo di oggetto; file3ds+offset puntano ad un chunk_id
// 0x4000 (EDIT_OBJECT)
var obj_size,aux:integer;
begin
 obj_size:=get_chunk_fullsize(file3DS, offset);

 aux:=find_first_subchunk_oftype(file3DS, offset, OBJ_CAMERA);
 if ((aux<>-1) and (aux<offset+obj_size)) then result:=CAMERA
 else begin
        aux:=find_first_subchunk_oftype(file3DS, offset, OBJ_LIGHT);
        if ((aux<>-1) and (aux<offset+obj_size)) then result:=LIGHT
        else begin
               aux:=find_first_subchunk_oftype(file3DS, offset, OBJ_TRIMESH);
               if ((aux<>-1) and (aux<offset+obj_size)) then result:=NORMALOBJ
                                                        else result:=-1; // oggetto sconosciuto
             end;
      end;
end;


end.
