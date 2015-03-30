#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "kancr.h"
#include <stdlib.h>

#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

float fov = 90.f;
float hpc;

void setup_frustum( float fov );

void set_fov( float input ){
	fov = input;
	hpc = (WIDTH>>1)/(float)tan((fov*0.5f)*(M_PI/180));
	setup_frustum( fov );
}

void init_kancr(){
	set_fov( 90.f );
}

object *make_object(
	vector *points, int pointcount,
	int *point_indices, int *uv_indices, int *material_indices, int facecount,
	float *texcoords, int texcoordcount,
	material *materials, int materialcount)
{
	int i;
	object *temp = (object*)malloc(sizeof(object));
	face *temp_faces = (face*)malloc(sizeof(face)*facecount);

	memset( temp, 0, sizeof(object) );

	temp->points = malloc( sizeof(vector)*pointcount );
	memcpy( temp->points, points, sizeof(vector)*pointcount );
	temp->pointcount = pointcount;

	temp->texcoords = malloc( sizeof(texcoord)*texcoordcount );

	for( i=0; i<texcoordcount; i++ ){
		temp->texcoords[i].u = texcoords[i*2];
		temp->texcoords[i].v = texcoords[i*2+1];
	}
	temp->texcoordcount = texcoordcount;

#ifdef _DEBUG
	printf("materialcount: %i\n", materialcount );
#endif

	if(materialcount!=0){
		temp->materials = malloc( sizeof(material)*materialcount );
		temp->materialcount = materialcount;
		if(materials!=NULL) memcpy( temp->materials, materials, sizeof(material)*materialcount );
	}else{
		temp->materials = malloc( sizeof(material) );
		temp->materialcount = 1;
	}

	temp->normals = malloc( sizeof(vector)*pointcount );
	temp->normalcount = pointcount;

	memset(temp_faces, 0, sizeof(face)*facecount );

	for( i=0; i<facecount; i++ ){
		temp_faces[i].vertex[0].index = point_indices[i*3];
		temp_faces[i].vertex[1].index = point_indices[i*3+1];
		temp_faces[i].vertex[2].index = point_indices[i*3+2];
		temp_faces[i].vertex[0].normal = point_indices[i*3];
		temp_faces[i].vertex[1].normal = point_indices[i*3+1];
		temp_faces[i].vertex[2].normal = point_indices[i*3+2];
		temp_faces[i].vertex[0].texcoord = uv_indices[i*3];
		temp_faces[i].vertex[1].texcoord = uv_indices[i*3+1];
		temp_faces[i].vertex[2].texcoord = uv_indices[i*3+2];
		if(material_indices) temp_faces[i].material = material_indices[i];
		else temp_faces[i].material = 0;
	}

	temp->faces = temp_faces;
	temp->facecount = facecount;

	generate_normals( temp );
	temp->bounding_sphere = bounding_sphere( temp );
	temp->recalc_bounding_sphere = FALSE;

	return temp;
}

object *copy_object( object *source ){
	object *temp = malloc(sizeof(object));

	temp->faces = malloc(sizeof(face)*source->facecount);
	memcpy( temp->faces, source->faces, sizeof(face)*source->facecount);
	temp->facecount = source->facecount;

	temp->points = malloc(sizeof(vector)*source->pointcount);
	memcpy( temp->points, source->points, sizeof(vector)*source->pointcount);
	temp->pointcount = source->pointcount;

	temp->materials = malloc(sizeof(material)*source->materialcount);
	memcpy( temp->materials, source->materials, sizeof(material)*source->materialcount);
	temp->materialcount = source->materialcount;

	temp->normals = malloc(sizeof(vector)*source->normalcount);
	memcpy( temp->normals, source->normals, sizeof(vector)*source->normalcount);
	temp->normalcount = source->normalcount;

	temp->texcoords = malloc(sizeof(texcoord)*source->texcoordcount);
	memcpy( temp->texcoords, source->texcoords, sizeof(texcoord)*source->texcoordcount);
	temp->texcoordcount = source->texcoordcount;

	return temp;
}

void free_object( object *object ){
	free( object->points );
	free( object->faces );
	free( object->materials );
	free( object->normals );
}

void generate_normals( object *object ){
	int i;
	vector* vert;

	face* face = object->faces;
	for( i=object->facecount; i; i-- ){
		face->normal = vector_normalize(
						vector_crossproduct(
							vector_sub( object->points[face->vertex[2].index], object->points[face->vertex[0].index] ),
							vector_sub( object->points[face->vertex[1].index], object->points[face->vertex[0].index] )
							)
						);
		face++;
	}

	memset( object->normals, 0, sizeof(vector)*object->normalcount );
	face = object->faces;
	for(i=object->facecount; i; i--){
		object->normals[face->vertex[0].normal] =  vector_add( object->normals[face->vertex[0].normal], face->normal );
		object->normals[face->vertex[1].normal] =  vector_add( object->normals[face->vertex[1].normal], face->normal );
		object->normals[face->vertex[2].normal] =  vector_add( object->normals[face->vertex[2].normal], face->normal );
		face++;
	}

	vert = object->normals;
	for( i=0; i<object->normalcount; i++ ){
		*vert = vector_normalize( *vert );
		vert++;
	}

	object->recalc_normals = FALSE;
}

float bounding_sphere(object *object ){
	int counter;
	vector *points = object->points;
	float max = 0;

	for(counter=object->pointcount;counter;counter--){
		vector v = *points++;
		float rad = (v.x*v.x)+(v.y*v.y)+(v.z*v.z);
		if( rad>max ) max = rad;
	}

	return (float)sqrt( max );
}

extern unsigned int zbuffer[WIDTH*HEIGHT];

void start_frame(){
	memset( zbuffer, 0, sizeof(unsigned int)*WIDTH*HEIGHT);
}

void set_camera( camera *cam, vector pos, vector target, float roll ){
	cam->position = pos;
	cam->target = target;
	cam->roll = roll;
	matrix_lookat( cam->matrix, pos, target, roll );
}



struct{
	plane sides[4];
	plane znear;
	plane zfar;
}frustum;

void setup_frustum( float fov ){
	float angle_horizontal = (float)atan2( (float)WIDTH/2, hpc )-0.0001f;
	float angle_vertical = (float)atan2( (float)HEIGHT/2, hpc )-0.0001f;
	float sh = (float)sin(angle_horizontal);
	float sv = (float)sin(angle_vertical);
	float ch = (float)cos(angle_horizontal);
	float cv = (float)cos(angle_vertical);

	float angle_horizontal2 = (float)atan2( ((float)WIDTH+1.f)/2, hpc )-0.0001f;
	float ch2 = (float)cos(angle_horizontal2);

	frustum.sides[0].normal = vector_make(ch,  0, sh);
	frustum.sides[0].distance = 0;
	frustum.sides[1].normal = vector_make(-ch2,  0, sh);
	frustum.sides[1].distance = 0;
	frustum.sides[2].normal = vector_make( 0, cv, sv);
	frustum.sides[2].distance = 0;
	frustum.sides[3].normal = vector_make( 0, -cv, sv);
	frustum.sides[3].distance = 0;
	frustum.znear.normal = vector_make(0, 0, 1);
	frustum.znear.distance = 2.01f;
}

#define MAXVERTS 32
typedef struct{
	float r;
	float g;
	float b;
	float a;
}rgba;

typedef struct{
	vector points[MAXVERTS];
	vector normals[MAXVERTS];
	texcoord texcoords[MAXVERTS];
	texcoord envcoords[MAXVERTS];
	rgba rgbas[MAXVERTS];
	int vertexcount;
}polygon;
polygon polygons[2];

_inline polygon *clip_polygon( plane *plane, polygon *in_poly, polygon *out_poly){
	float curr_dot, next_dot;
	BOOL curr_in, next_in;
	int counter;

	out_poly->vertexcount = 0;
	curr_dot = vector_dotproduct( in_poly->points[0], plane->normal );
	curr_in = (curr_dot>=plane->distance);

	for(counter=0; counter<in_poly->vertexcount; counter++){
		int next = (counter+1)%in_poly->vertexcount;
		vector curr_point = in_poly->points[counter];
		vector next_point = in_poly->points[next];

		if(curr_in){
			out_poly->points[out_poly->vertexcount] = curr_point;
			out_poly->texcoords[out_poly->vertexcount] = in_poly->texcoords[counter];
			out_poly->envcoords[out_poly->vertexcount] = in_poly->envcoords[counter];
			out_poly->vertexcount++;
		}

		next_dot = vector_dotproduct(next_point,plane->normal);
		next_in = (next_dot>=plane->distance);

		if(curr_in != next_in){
			float scale = (plane->distance-curr_dot)/(next_dot-curr_dot);
			vector vec = vector_add(
				vector_scale(
					vector_sub(next_point, curr_point),
					scale),
				curr_point
			);
			out_poly->points[out_poly->vertexcount] = vec;
			out_poly->texcoords[out_poly->vertexcount].u = in_poly->texcoords[counter].u+(in_poly->texcoords[next].u-in_poly->texcoords[counter].u)*scale;
			out_poly->texcoords[out_poly->vertexcount].v = in_poly->texcoords[counter].v+(in_poly->texcoords[next].v-in_poly->texcoords[counter].v)*scale;
			out_poly->envcoords[out_poly->vertexcount].u = in_poly->envcoords[counter].u+(in_poly->envcoords[next].u-in_poly->envcoords[counter].u)*scale;
			out_poly->envcoords[out_poly->vertexcount].v = in_poly->envcoords[counter].v+(in_poly->envcoords[next].v-in_poly->envcoords[counter].v)*scale;
			out_poly->rgbas[out_poly->vertexcount].r = in_poly->rgbas[counter].r+(in_poly->rgbas[next].r-in_poly->rgbas[counter].r)*scale;
			out_poly->rgbas[out_poly->vertexcount].g = in_poly->rgbas[counter].g+(in_poly->rgbas[next].g-in_poly->rgbas[counter].g)*scale;
			out_poly->rgbas[out_poly->vertexcount].b = in_poly->rgbas[counter].b+(in_poly->rgbas[next].b-in_poly->rgbas[counter].b)*scale;
			out_poly->rgbas[out_poly->vertexcount].a = in_poly->rgbas[counter].a+(in_poly->rgbas[next].a-in_poly->rgbas[counter].a)*scale;
			out_poly->vertexcount++;
		}

		curr_dot = next_dot;
		curr_in = next_in;
	}
	return out_poly;
}

_inline vertex_2d transform( vector v ){
	vertex_2d temp;
	float hpc = (WIDTH>>1)/(float)tan((fov*0.5f)*(M_PI/180));
	float muller = 1.0f/(v.z);
	temp.x = (WIDTH>>1)+(hpc*v.x*muller);
	temp.y = (HEIGHT>>1)-(hpc*v.y*muller);
	temp.z = v.z;
	return temp;
}

vector light = {0,4,5};

_inline void draw_triangle( unsigned int *screen, vector v[3], texcoord texcoords[3], texcoord envcoords[3], material *material, unsigned int color, rgba colors[3] ){
	vertex_2d v_2d[3];
	if( v[0].z>0.f&&v[1].z>0.f&&v[2].z>0.f ){
		v_2d[0] = transform( v[0] );
		v_2d[1] = transform( v[1] );
		v_2d[2] = transform( v[2] );

		switch( (material->rendermode)&0x7FFFFFFF ){
		case RENDER_FLATSHADE:
			draw_flat_triangle( screen, v_2d, color  );
			break;
		case RENDER_PHONGMAP:
		case RENDER_ENVMAP:
			draw_texture_triangle( screen, v_2d, envcoords, material->envmap  );
			break;
		case RENDER_TEXTUREMAP:
			draw_perfect_texture_triangle( screen, v_2d, texcoords, material->texture  );
			break;
		case RENDER_TEXTUREMAP|RENDER_BUMPMAP:
			draw_perfect_bump_texture_triangle( screen, v_2d, texcoords, envcoords, material->texture, material->envmap  );
			break;
		case RENDER_ENVBUMPMAP:
			draw_perfect_envbump_texture_triangle( screen, v_2d, texcoords, envcoords, material->texture, material->envmap  );
			break;
		case RENDER_TEXTUREMAP|RENDER_ENVMAP:
			draw_perfect_multi_texture_triangle( screen, v_2d, texcoords, envcoords, material->texture, material->envmap  );
			break;
		case (RENDER_TEXTUREMAP|RENDER_FLATSHADE):
			draw_perfect_texture_flat_triangle( screen, v_2d, texcoords, material->texture, color );
			break;
		default:
			draw_flat_triangle( screen, v_2d, color  );
			break;
		}
	}
}

/*
	FIKS OPP ALT ROTET HER!
	BRUK MATERIALER OG SJEKK RENDERINGSTYPE!
	MER CULLING!
*/
void draw_object( unsigned int *screen, object *object, camera *cam ){
	matrix m;
	matrix inverse;
	vector obj_cam;
	int counter;

	vector cam_light = light;//matrix_transformvector( cam->matrix, light );
	face *face = object->faces;

	matrix_identity( m );
	matrix_multiply( m, cam->matrix );
	matrix_multiply( m, object->matrix );

	matrix_transponate( inverse, object->matrix );

	obj_cam = cam->position;
//	obj_cam = matrix_transformvector( inverse, cam->position );

	//if(object->recalc_normals)
	generate_normals( object );
	//if(object->recalc_bounding_sphere) object->bounding_sphere = bounding_sphere( object );

	for(counter=0;counter<object->facecount;counter++){
		vector v[3], test, normal;
		texcoord texcoords[3], envcoords[3];
		material *material = &object->materials[face->material];

		v[0] = matrix_transformvector( m, object->points[face->vertex[0].index]);
		test = v[0];
		normal = matrix_rotatevector( m, face->normal );

		if( (material->rendermode&RENDER_DOUBLESIDED) || vector_dotproduct(normal,test)>=0.f ){
			float dot;
			float shade;
			unsigned int color;
			rgba colors[3];
			polygon *out;

//			v[0] = matrix_transformvector( m, object->points[face->vertex[0].index]);
			v[1] = matrix_transformvector( m, object->points[face->vertex[1].index]);
			v[2] = matrix_transformvector( m, object->points[face->vertex[2].index]);

			if( material->rendermode&RENDER_FLATSHADE ){
				normal = matrix_rotatevector( object->matrix, face->normal );
				dot = vector_dotproduct(
					vector_normalize(normal),
					vector_normalize(
						cam_light
					)
				);

				shade = (float)acos( dot )/M_PI;

				color = (int)(shade*255.f);
				color |= color<<8|color<<16;
			}

			if( material->rendermode&RENDER_GOURARDSHADE ){
				int i;
				for(i=0;i<3;i++){
					dot = vector_dotproduct(
					vector_normalize(object->normals[face->vertex[i].normal]),
					vector_normalize(
						vector_sub(
							cam_light,
							v[0] )
						)
					);

					shade = (float)acos( dot )/M_PI;
					colors[i].r = shade*255;
					colors[i].g = shade*255;
					colors[i].b = shade*255;
					colors[i].a = shade*255;
				}
			}

			if( material->rendermode&RENDER_ENVMAP || material->rendermode&RENDER_BUMPMAP || material->rendermode&RENDER_ENVBUMPMAP ){
				vector cam_normals[3];
				cam_normals[0] = vector_normalize( matrix_rotatevector( m, object->normals[face->vertex[0].normal] ));
				cam_normals[1] = vector_normalize( matrix_rotatevector( m, object->normals[face->vertex[1].normal] ));
				cam_normals[2] = vector_normalize( matrix_rotatevector( m, object->normals[face->vertex[2].normal] ));
				envcoords[0].u = 0.5f+cam_normals[0].x*0.5f;
				envcoords[0].v = 0.5f-cam_normals[0].y*0.5f;
				envcoords[1].u = 0.5f+cam_normals[1].x*0.5f;
				envcoords[1].v = 0.5f-cam_normals[1].y*0.5f;
				envcoords[2].u = 0.5f+cam_normals[2].x*0.5f;
				envcoords[2].v = 0.5f-cam_normals[2].y*0.5f;
			}
			if( material->rendermode&RENDER_PHONGMAP ){

				// dette er fake-phong'ing, trenger litt flere triks for å funke SKIKKELIG. (fjerne bakside-lyslegging osv :P)
				// tror 3dica forklarer detta
				vector cam_normals[3];
				cam_normals[0] = vector_normalize( matrix_rotatevector( object->matrix, object->normals[face->vertex[0].normal] ));
				cam_normals[1] = vector_normalize( matrix_rotatevector( object->matrix, object->normals[face->vertex[1].normal] ));
				cam_normals[2] = vector_normalize( matrix_rotatevector( object->matrix, object->normals[face->vertex[2].normal] ));

				envcoords[0].u = 0.5f+cam_normals[0].x*0.5f;
				envcoords[0].v = 0.5f-cam_normals[0].y*0.5f;
				envcoords[1].u = 0.5f+cam_normals[1].x*0.5f;
				envcoords[1].v = 0.5f-cam_normals[1].y*0.5f;
				envcoords[2].u = 0.5f+cam_normals[2].x*0.5f;
				envcoords[2].v = 0.5f-cam_normals[2].y*0.5f;
			}
			if( material->rendermode&RENDER_TEXTUREMAP || material->rendermode&RENDER_ENVBUMPMAP ){
				texcoords[0] = object->texcoords[face->vertex[0].texcoord];
				texcoords[1] = object->texcoords[face->vertex[1].texcoord];
				texcoords[2] = object->texcoords[face->vertex[2].texcoord];
			}

			polygons[0].points[0] = v[0];
			polygons[0].points[1] = v[1];
			polygons[0].points[2] = v[2];
			polygons[0].vertexcount = 3;
			polygons[0].texcoords[0].u = texcoords[0].u*255;
			polygons[0].texcoords[0].v = texcoords[0].v*255;
			polygons[0].texcoords[1].u = texcoords[1].u*255;
			polygons[0].texcoords[1].v = texcoords[1].v*255;
			polygons[0].texcoords[2].u = texcoords[2].u*255;
			polygons[0].texcoords[2].v = texcoords[2].v*255;

			polygons[0].envcoords[0].u = envcoords[0].u*255;
			polygons[0].envcoords[0].v = envcoords[0].v*255;
			polygons[0].envcoords[1].u = envcoords[1].u*255;
			polygons[0].envcoords[1].v = envcoords[1].v*255;
			polygons[0].envcoords[2].u = envcoords[2].u*255;
			polygons[0].envcoords[2].v = envcoords[2].v*255;

			polygons[0].rgbas[0] = colors[0];
			polygons[0].rgbas[1] = colors[1];
			polygons[0].rgbas[2] = colors[2];

			out = clip_polygon( &frustum.sides[0], &polygons[0], &polygons[1] );
			out = clip_polygon( &frustum.sides[1], &polygons[1], &polygons[0] );
			out = clip_polygon( &frustum.sides[2], &polygons[0], &polygons[1] );
			out = clip_polygon( &frustum.sides[3], &polygons[1], &polygons[0] );
			out = clip_polygon( &frustum.znear, &polygons[0], &polygons[1] );

			if(out->vertexcount==3){
				v[0] = out->points[0];
				v[1] = out->points[1];
				v[2] = out->points[2];
				texcoords[0] = out->texcoords[0];
				texcoords[1] = out->texcoords[1];
				texcoords[2] = out->texcoords[2];
				envcoords[0] = out->envcoords[0];
				envcoords[1] = out->envcoords[1];
				envcoords[2] = out->envcoords[2];
				colors[0] = polygons[0].rgbas[0];
				colors[1] = polygons[0].rgbas[1];
				colors[2] = polygons[0].rgbas[2];
				draw_triangle( screen, v, texcoords, envcoords, material, color, colors );

			}else if(out->vertexcount>3){
				int i;
				for( i=1; i<(out->vertexcount-1); i++){
					v[0] = out->points[0];
					v[1] = out->points[i];
					v[2] = out->points[i+1];
					texcoords[0] = out->texcoords[0];
					texcoords[1] = out->texcoords[i];
					texcoords[2] = out->texcoords[i+1];
					envcoords[0] = out->envcoords[0];
					envcoords[1] = out->envcoords[i];
					envcoords[2] = out->envcoords[i+1];
					colors[0] = polygons[0].rgbas[0];
					colors[1] = polygons[0].rgbas[1];
					colors[2] = polygons[0].rgbas[2];
					draw_triangle( screen, v, texcoords, envcoords, material, color, colors );
				}
			}
		}
		face++;
	}
}