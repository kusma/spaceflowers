#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "kancr.h"
#include "file.h"

#define CHUNK_FORM 0x4d524f46
#define CHUNK_TAGS 0x53474154
#define CHUNK_LAYR 0x5259414c // parser ikke denne nå
#define CHUNK_PNTS 0x53544e50
#define CHUNK_BBOX 0x584f4242 // parser ikke denne nå
#define CHUNK_POLS 0x534c4f50
#define CHUNK_PTAG 0x47415450 // parser ikke ennå
#define CHUNK_SURF 0x46525553 // parser ikke ennå
#define CHUNK_VMAP 0x50414d56
#define CHUNK_VMAD 0x44414d56

#define PARAMETER_LWO2 0x324f574c
#define PARAMETER_FACE 0x45434146
#define PARAMETER_SURF 0x46525553
#define PARAMETER_TXUV 0x56555854

unsigned int swap_int(unsigned int integer){
	return	((integer&0xFF000000)>>24)|
				((integer&0x00FF0000)>>8 )|
				((integer&0x0000FF00)<<8 )|
				((integer&0x000000FF)<<24);
}

unsigned int swap_short(unsigned short integer){
	return	((integer&0xFF00)>>8)|
			((integer&0x00FF)<<8 );
}

// allignment er ikke gunstig her...
#pragma pack(1)
struct{
	unsigned int tag;
	unsigned int len;
}chunk;

struct{
	unsigned int tag;
	unsigned short len;
}subchunk;
#pragma pack()

#define MAX_TAGS 256
#define MAX_FACES 65535
#define MAX_UVS	65535
#define MAX_POINTS 65535



char *tags[MAX_TAGS];
int tagcount = 0;

vector points[MAX_POINTS];
int pointcount = 0;


typedef struct{
	int vert;
	int poly;
	int index;
}vmad_entry;

int vmap[MAX_POINTS];
vmad_entry vmad[MAX_FACES*3];
int vmadcount = 0;

float uvs[MAX_UVS][2];
int uvcount = 0;

int uv_index[MAX_FACES][3];
int vertex_index[MAX_FACES][3];
int material_index[MAX_FACES];
int facecount = 0;
BOOL discontineous = FALSE;


_inline int find_vertex(int face, int vertex){
	if(vertex_index[face][0]==vertex) return 0;
	if(vertex_index[face][1]==vertex) return 1;
	if(vertex_index[face][2]==vertex) return 2;
#ifdef _DEBUG
	printf("VMAD_ERROR\n");
#endif
	return 0;
}

unsigned int read_vx( file *file, unsigned int *i ){
	unsigned short s;

	file_read( &s, sizeof(unsigned short), 1, file);
	s = swap_short(s);
	if(s<0xFF00){
		*i = s;
		return 2;
	};
	*i = s<<16;
	file_read( &s, sizeof(unsigned short), 1, file);
	s = swap_short(s);
	*i |= s;
	*i -= 0xFF000000;
	return 4;
}

char *read_string( file *file, int *bytes_eaten ){
	int motthafuck=0;
	int i;
	int temp;
	unsigned char buffer[256];
	for(i=0;i<256;i++){
		file_read( &buffer[i], 1, 1, file );
		motthafuck++;
		if( buffer[i]=='\0' ) break;
	}
	if((strlen(buffer)+1)&1){
		file_read( &temp, 1, 1, file);
		motthafuck++;
	}
	*bytes_eaten = motthafuck;
	return strdup(buffer);
}

void read_tags_chunk( file *file, int len ){
	int i;
	char *string = (char*)malloc(sizeof(char)*(len+1));
	file_read( &string[1], len, 1, file );
	string[0] = '\0';

	for(i=0;i<len-1;i++)
		if(string[i]=='\0')
			tags[tagcount++] = &string[i+1];
}

void read_pnts_chunk( file *file, int len ){
	int i;
	pointcount = len / 12;
	file_read( points, sizeof(vector), pointcount, file );

	for(i=0; i<pointcount; i++){
		unsigned int temp;
		temp = swap_int(*(unsigned int*)&points[i].x);
		points[i].x = *((float*)&temp);
		temp = swap_int(*(unsigned int*)&points[i].y);
		points[i].y = *((float*)&temp);
		temp = swap_int(*(unsigned int*)&points[i].z);
		points[i].z = *((float*)&temp);
	}
}

void read_pols_chunk( file *file, int len ){
	unsigned int type;
	int i;
	file_read( &type, sizeof(unsigned int), 1, file );
	len -= sizeof(unsigned int);

	if(type != PARAMETER_FACE){
#ifdef _DEBUG
		printf("\n*** WRONG TYPE OF POLS ***\n\n");
#endif
		file_seek( file, len, SEEK_CUR );
		return;
	}

	i = len;
	while(i>0){
		unsigned short flags;
		file_read( &flags, sizeof(unsigned short), 1, file );
		i -= sizeof(unsigned short);
		flags = swap_short( flags );
		flags &= 0x03FF;

		if( flags != 3 ){
			printf("%u is too many vertices! (TRIANGULATE, BITCH)\n", flags);
			file_seek( file, sizeof(unsigned short)*flags, SEEK_CUR );
		}else{
			unsigned short temp[3];
			// TODO: fiks så den bruker VX-saker her
			file_read( temp, sizeof(unsigned short), flags, file );
			if( facecount<MAX_FACES ){
				vertex_index[facecount][0] = swap_short(temp[0]);
				vertex_index[facecount][1] = swap_short(temp[1]);
				vertex_index[facecount][2] = swap_short(temp[2]);
				facecount++;
#ifdef _DEBUG
				printf("faces: %u\r", facecount);
#endif
			}
#ifdef _DEBUG
			else printf("too many vertex_index, skipping...\n");
#endif
		}
		i -= sizeof(unsigned short)*flags;
	}
#ifdef _DEBUG
	printf("\n");
#endif
}

void read_ptag_chunk( file *file, int len ){
	unsigned int type;
	file_read( &type, sizeof(unsigned int), 1, file );
	len -= sizeof(unsigned int);

	if(type != PARAMETER_SURF){
		file_seek( file, len, SEEK_CUR );
#ifdef _DEBUG
		printf("*** unsupported PTAG ***\n");
#endif
	}

	while(len>0){
		unsigned short tag;
		int index;
		len -= read_vx( file, &index )+2;
		file_read( &tag, sizeof(unsigned short), 1, file );
		tag = swap_short( tag );
		material_index[index] = tag;
	}
}

void read_vmap_chunk( file *file, int len ){
	unsigned int type;
	unsigned short dimension;
	char *name;
	int bytes_eaten;
	int count = 0;

	file_read( &type, sizeof(unsigned int), 1, file );
	len -= sizeof(unsigned int);
	file_read( &dimension, sizeof(unsigned short), 1, file );
	dimension = swap_short( dimension );
	len -= sizeof(unsigned short);

	if(type!=PARAMETER_TXUV||dimension!=2){
		file_seek( file, len, SEEK_CUR );
#ifdef _DEBUG
		printf("*** unsupported VMAP ***\n");
#endif
	}

	discontineous = FALSE;

	name = read_string( file, &bytes_eaten );
	len -= bytes_eaten;
#ifdef _DEBUG
	printf( "VMAP name: %s\n", name );
#endif
	while(len>0){
		int index;
		unsigned int temp;
		float in_uv[2];
		len -= read_vx( file, &index );

		file_read( &in_uv, sizeof(float), 2, file );
		len -= sizeof(float)*2;

		temp = swap_int(*(unsigned int*)&in_uv[0]);
		in_uv[0] = *((float*)&temp);
		temp = swap_int(*(unsigned int*)&in_uv[1]);
		in_uv[1] = *((float*)&temp);

		uvs[uvcount][0] = in_uv[0];
		uvs[uvcount][1] = in_uv[1];
		vmap[index] = uvcount;
		uvcount++;
	}
}

void read_vmad_chunk( file *file, int len ){
	unsigned int type;
	unsigned short dimension;
	char *name;
	int bytes_eaten;

	file_read( &type, sizeof(unsigned int), 1, file );
	len -= sizeof(unsigned int);
	file_read( &dimension, sizeof(unsigned short), 1, file );
	dimension = swap_short( dimension );
	len -= sizeof(unsigned short);

	if(type!=PARAMETER_TXUV||dimension!=2){
		file_seek( file, len, SEEK_CUR );
#ifdef _DEBUG
		printf("*** unsupported VMAD ***\n");
#endif
	}

	name = read_string( file, &bytes_eaten );
	len -= bytes_eaten;

#ifdef _DEBUG
	printf( "VMAD name: %s\n", name );
#endif

	while(len>0){
		int vertex_index;
		int poly_index;
		unsigned int temp;
		float in_uv[2];
		len -= read_vx( file, &vertex_index );
		len -= read_vx( file, &poly_index );

		file_read( &in_uv, sizeof(float), 2, file );
		len -= sizeof(float)*2;

		temp = swap_int(*(unsigned int*)&in_uv[0]);
		in_uv[0] = *((float*)&temp);
		temp = swap_int(*(unsigned int*)&in_uv[1]);
		in_uv[1] = *((float*)&temp);

		uvs[uvcount][0] = in_uv[0];
		uvs[uvcount][1] = in_uv[1];
		vmad[vmadcount].index = uvcount++;
		vmad[vmadcount].poly = poly_index;
		vmad[vmadcount].vert = vertex_index;
		vmadcount++;
	}
}

void read_surf_chunk( file *file, int len ){
	char *source;
	char *name;
	int bytes_eaten;
	name = read_string( file, &bytes_eaten );
	len -= bytes_eaten;
	source = read_string( file, &bytes_eaten );
	len -= bytes_eaten;
#ifdef _DEBUG
	printf("surface attributes for %s\n", name );
#endif
	while(len>0){
		file_read( &subchunk, sizeof(subchunk), 1, file );
#ifdef _DEBUG
		printf("subchunk.len = %i\n\n", swap_short(subchunk.len));
#endif
		switch( subchunk.tag ){

		default:
			file_seek( file, swap_short(subchunk.len), SEEK_CUR );
#ifdef _DEBUG
			printf("unhandled subchunk: 0x%x aka %c%c%c%c\n",subchunk.tag,
				((char*)&subchunk.tag)[0],
				((char*)&subchunk.tag)[1],
				((char*)&subchunk.tag)[2],
				((char*)&subchunk.tag)[3]);
#endif
			break;
		}
		len -= swap_int(subchunk.len);
	}
}

object *load_lwo(char *filename){
	unsigned int type;
	int len;
	int i;
	file* file = file_open(filename);
	if(file==NULL){
#ifdef _DEBUG
		printf("file not found\n");
#endif
		return NULL;
	}

	tagcount = 0;
	pointcount = 0;
	facecount = 0;
	uvcount = 0;
	vmadcount = 0;

	memset( uv_index, 0, sizeof(unsigned int)*3*MAX_UVS );

	file_read( &chunk, sizeof(chunk), 1, file );
	len = swap_int( chunk.len );

	if( chunk.tag != CHUNK_FORM ){
#ifdef _DEBUG
		printf("seems not to be an LWO-file\n");
#endif
		file_close( file );
		return NULL;
	}

	file_read( &type, sizeof(unsigned int), 1, file );
	if( type != PARAMETER_LWO2 ){
#ifdef _DEBUG
		printf("unsupported LWO\n");
#endif
		file_close( file );
		return NULL;
	}

	/* filen er gyldig, ready to go */

	while(len>0){
		file_read( &chunk, sizeof(chunk), 1, file );

		switch( chunk.tag ){
		case CHUNK_TAGS:
			read_tags_chunk( file, swap_int(chunk.len) );
			break;
		case CHUNK_PNTS:
			read_pnts_chunk( file, swap_int(chunk.len) );
			break;
		case CHUNK_POLS:
			read_pols_chunk( file, swap_int(chunk.len) );
			break;
		case CHUNK_PTAG:
			read_ptag_chunk( file, swap_int(chunk.len) );
			break;
		case CHUNK_VMAP:
			read_vmap_chunk( file, swap_int(chunk.len) );
			break;
		case CHUNK_VMAD:
			read_vmad_chunk( file, swap_int(chunk.len) );
			break;
		case CHUNK_SURF:
			read_surf_chunk( file, swap_int(chunk.len) );
			break;
		default:
			file_seek( file, swap_int(chunk.len), SEEK_CUR );
#ifdef _DEBUG
			printf("unhandled chunk: 0x%x aka %c%c%c%c\n",chunk.tag,
				((char*)&chunk.tag)[0],
				((char*)&chunk.tag)[1],
				((char*)&chunk.tag)[2],
				((char*)&chunk.tag)[3]);
#endif
			break;
		}
//		printf("len: %u\n", len);
		len -= swap_int(chunk.len);
	}

	// generere uv-tabeller!
	for( i=0; i<facecount; i++ ){
		uv_index[i][0] = vmap[ vertex_index[i][0] ];
		uv_index[i][1] = vmap[ vertex_index[i][1] ];
		uv_index[i][2] = vmap[ vertex_index[i][2] ];
	}

	for( i=0; i<vmadcount; i++ ){
		uv_index[vmad[i].poly][find_vertex(vmad[i].poly, vmad[i].vert)] = vmad[i].index;
	}

	file_close( file );

	return make_object(
	points, pointcount,
	(int*)vertex_index,
	(int*)uv_index,
	material_index, facecount,
	(float*)uvs, uvcount,
	NULL, 0);

}