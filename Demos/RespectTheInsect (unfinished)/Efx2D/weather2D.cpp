#include "weather2D.h"
#include "..\utility.h"
#include <math.h>
#include <time.h>


extern HWND miawin;  // finestra principale; usata qui
                     // per le debug_error

void weather2D::randomize_particle(int num)
{
	cl[num].pos_x=float(image->width)*rand()/RAND_MAX;
	cl[num].pos_y=float(image->height)*rand()/RAND_MAX;
	cl[num].vel_x=4.0f*rand()/RAND_MAX-2.0f;
	cl[num].vel_y=4.0f*rand()/RAND_MAX-2.0f;
	cl[num].force=20.0f*rand()/RAND_MAX;
	cl[num].mass=20.0f*rand()/RAND_MAX;
	cl[num].attenuation=0.9f+0.09f*rand()/RAND_MAX;
	cl[num].iner=0.1f+0.9f*rand()/RAND_MAX;
	cl[num].life=1000+int(1000.0f*rand()/RAND_MAX);
}

void weather2D::init()
{ 
	int x,y;
	double value;

	srand( (unsigned)time( NULL ) );


	particle_map=(unsigned char *)malloc(WEATHER_PARTICLE_SIZE*WEATHER_PARTICLE_SIZE);

	for (y=0; y<WEATHER_PARTICLE_SIZE; y++)
		for (x=0; x<WEATHER_PARTICLE_SIZE;  x++)
		{
			value=(1.0-sqrt(1+sqr(x-WEATHER_PARTICLE_SIZE/2)+sqr(y-WEATHER_PARTICLE_SIZE/2))/(WEATHER_PARTICLE_SIZE/2))*WEATHER_INTENSITY;
			if (value<0.0f) value=0.0f;
			if (value>255.0f) value=255.0f;
			particle_map[x+y*WEATHER_PARTICLE_SIZE]=
				unsigned char (value);
		}

	/*
	for (y=0; y<HEROX_PARTICLE_SIZE; y++)
	{
		particle_map[0+y*HEROX_PARTICLE_SIZE]=0xF;
		particle_map[HEROX_PARTICLE_SIZE-1+y*HEROX_PARTICLE_SIZE]=0xF;
		particle_map[y]=0xF;
		particle_map[y+(HEROX_PARTICLE_SIZE-1)*HEROX_PARTICLE_SIZE]=0xF;
	}
	*/
	
	for (x=0; x<WEATHER_PARTICLE_NUM; x++) randomize_particle(x);

	for (x=0; x<WEATHER_ZONE_NUM; x++)
	{
		zo[x].pos_x=image->width*rand()/RAND_MAX;
		zo[x].pos_y=image->height*rand()/RAND_MAX;
		zo[x].force=(4.0f*rand()/RAND_MAX)-2.0f;
	}
} 

void weather2D::paint_particle(int x,int y)
{	
	int ix,iy,startx,starty;
	int dispx,dispy;
	int color;
	int r,g,b;

	startx=x-WEATHER_PARTICLE_SIZE/2;
	starty=y-WEATHER_PARTICLE_SIZE/2;

	if ((startx>=0) && (x<image->width-WEATHER_PARTICLE_SIZE/2)
		&& (starty>=0) && (y<image->height-WEATHER_PARTICLE_SIZE/2))
	{
		for (iy=0; iy<WEATHER_PARTICLE_SIZE; iy++)
			for (ix=0; ix<WEATHER_PARTICLE_SIZE; ix++)
			{	
				/*dispx=0;
				dispy=0;
				
				if ((ix>0) && (ix<WEATHER_PARTICLE_SIZE-1))
				dispx=int(particle_map[ix+1+iy*WEATHER_PARTICLE_SIZE])-int(particle_map[ix-1+iy*WEATHER_PARTICLE_SIZE]);
		
				if ((iy>0) && (iy<WEATHER_PARTICLE_SIZE-1))
				dispy=int(particle_map[ix+(iy+1)*WEATHER_PARTICLE_SIZE])-int(particle_map[ix+(iy-1)*WEATHER_PARTICLE_SIZE]);

				dispx>>=4;
				dispy>>=4;
*/
				//dispx=dispy=0;

				color=image->uint32ptr[startx+ix+image->muly[starty+iy]];
				//	((image->uint32ptr[startx+ix+image->muly[starty+iy]]& 0xfefefe)>>1)
				//	+ ((image->uint32ptr[startx+ix+image->muly[starty+iy]] & 0xfefefe)>>1);
					
				b=color & 0xFF;
				g=(color & 0xFF00)>>8;
				r=(color & 0xFF0000)>>16;
				/*
				r-=particle_map[ix+iy*HEROX_PARTICLE_SIZE];
				g-=particle_map[ix+iy*HEROX_PARTICLE_SIZE];
				b-=particle_map[ix+iy*HEROX_PARTICLE_SIZE];
				if (r<0) r=0;
				if (g<0) g=0;
				if (b<0) b=0;
				*/
				r+=particle_map[ix+iy*WEATHER_PARTICLE_SIZE];
				g+=particle_map[ix+iy*WEATHER_PARTICLE_SIZE];
				b+=particle_map[ix+iy*WEATHER_PARTICLE_SIZE];
				if (r>255) r=255;
				if (g>255) g=255;
				if (b>255) b=255;
				
				color=b+(g<<8)+(r<<16);
				image->uint32ptr[startx+ix+image->muly[starty+iy]]=color;
			//		((color & 0xfefefe)>>1)
			//		+ ((image->uint32ptr[startx+ix+image->muly[starty+iy]] & 0xfefefe)>>1);
			}
	
	//	image->uint32ptr[x+image->muly[y]]=0; //per testare sta rullata
	}


}

void weather2D::do_simulation()
{
	int i,j;
	float dist,dx,dy,fx,fy,fcoriolis;

	for(i=0; i<WEATHER_PARTICLE_NUM; i++)
	{
		if (cl[i].life<=0) 
		{
			randomize_particle(i);
			continue;
		}

		fx=fy=0;

		for (j=0; j<WEATHER_ZONE_NUM; j++)
		{
			dx=zo[j].pos_x-cl[i].pos_x;
			dy=zo[j].pos_y-cl[i].pos_y;
			dist=sqrt(dx*dx+dy*dy);
			//if (dist<50.0f) continue;
			dx/=dist;
			dy/=dist;
			fx+=dx/(dist)*cl[i].force*zo[j].force;
			fy+=dy/(dist)*cl[i].force*zo[j].force;
		
		}
		cl[i].vel_x=cl[i].vel_x*cl[i].attenuation+(fx/cl[i].mass)*cl[i].iner;//cl[i].attenuation;
		cl[i].vel_y=cl[i].vel_y*cl[i].attenuation+(fy/cl[i].mass)*cl[i].iner;//cl[i].attenuation;
			
		cl[i].vel_x+=cl[i].vel_y*cos((cl[i].pos_y*PI)/float(image->height))*WEATHER_CORIOLIS_FORCE_INTENSITY;
		//cl[i].vel_x+=cl[i].vel_y*(fabs((cl[i].pos_y-float(image->height))/2.0f))/(float(image->height)/2.0f)*WEATHER_CORIOLIS_FORCE_INTENSITY;
	
		
		if (cl[i].vel_x>6.0f) cl[i].vel_x=6.0f;
		if (cl[i].vel_x<-6.0f) cl[i].vel_x=-6.0f;
		if (cl[i].vel_y>6.0f) cl[i].vel_y=6.0f;
		if (cl[i].vel_y<-6.0f) cl[i].vel_y=-6.0f;

		cl[i].pos_x+=cl[i].vel_x;
		cl[i].pos_y+=cl[i].vel_y;
		cl[i].life--;
	}
}

void weather2D::do_efx2D(double pos)
{
	int i;

	for(i=0; i<2; i++) do_simulation();

	for(i=0; i<WEATHER_PARTICLE_NUM; i++)
	{
		paint_particle(cl[i].pos_x,cl[i].pos_y);
	}

	if (steps%50==0)
	for (i=0; i<WEATHER_ZONE_NUM; i++)
	{
		zo[i].pos_x=image->width*rand()/RAND_MAX;
		zo[i].pos_y=image->height*rand()/RAND_MAX;
		zo[i].force=(4.0f*rand()/RAND_MAX)-2.0f;
		
	//	zo[i].pos_x=image->width/2;
	//	zo[i].pos_y=image->height/2;
	//	zo[i].force=2.0f;
		//image->clear(0);
	}
	steps++;

}