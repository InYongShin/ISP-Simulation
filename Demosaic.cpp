
#include "Demosaicer.hpp"


void Demosaicer::linearDemosaicRGGB() {
	
	float* bayers = new float[width*height];
	memcpy(bayers, data, width*height*sizeof(float));

	free(data);
	data = nullptr;
	data = new float[width*height*3];

	for(int i=0; i<height; ++i){
		for(int j=0; j<width; ++j){
			float r=0, g=0, b=0;
			if(i==0 && j==0){ // Left Top
				r =  bayers[width *  i    +  j   ];
				g = (bayers[width *  i    + (j+1)] + bayers[width * (i+1) + j   ]) / 2;
				b =  bayers[width * (i+1) + (j+1)];
				continue;
			}
			if(i==0 && j==width-1){ // Right Top
				r = bayers[width *  i    + (j-1)];
				g = bayers[width *  i    +  j   ];
				b = bayers[width * (i+1) +  j   ];
				continue;
			}
			if(i==height-1 && j==0){ // Left Bottom
				r =  bayers[width * (i-1) +  j   ];
				g =  bayers[width *  i    +  j   ];
				b =  bayers[width *  i    + (j+1)];
				continue;
			}
			if (i==height-1 && j==width-1) { // Right Bottom
				r =  bayers[width * (i-1) + (j-1)];
				g = (bayers[width *  i    + (j-1)] + bayers[width * (i-1) + j   ]) / 2;
				b =  bayers[width *  i    +  j   ];
				continue;
			}

			if(i==0){ // Top
				if(j%2==0){
					r =  bayers[width *  i    +  j   ];
					g = (bayers[width *  i    + (j-1)] + bayers[width * (i+1) +  j   ] + bayers[width * i    + (j+1)]) / 3;
					b = (bayers[width * (i+1) + (j-1)] + bayers[width * (i+1) + (j+1)]) / 2;
				}
				else{
					r = (bayers[width *  i    + (j-1)] + bayers[width *  i    + (j+1)]) / 2;
					g =  bayers[width *  i    +  j   ];
					b =  bayers[width * (i+1) +  j   ];
				}
				continue;
			}
			else if(i==height-1){ // Bottom
				if(j%2==0){
					r =  bayers[width * (i-1) +  j   ];
					g =  bayers[width *  i    +  j   ];
					b = (bayers[width *  i    + (j-1)] + bayers[width *  i    + (j+1)]) / 2;
				}
				else{
					r = (bayers[width * (i-1) + (j-1)] + bayers[width * (i-1) + (j+1)]) / 2;
					g = (bayers[width *  i    + (j-1)] + bayers[width * (i-1) +  j   ] + bayers[width * i    + (j+1)]) / 3;
					b =  bayers[width *  i    +  j   ];
				}
				continue;
			}
			
			if(j==0){ // Left
				if(i%2==0){
					r =  bayers[width *  i    +  j   ];
					g = (bayers[width * (i-1) +  j   ] + bayers[width * (i+1) +  j   ]) / 2;
					b = (bayers[width * (i-1) + (j+1)] + bayers[width * (i+1) + (j+1)]) / 2;
				}
				else{
					r = (bayers[width * (i-1) +  j   ] + bayers[width * (i+1) +  j   ]) / 2;
					g =  bayers[width *  i    +  j   ];
					b =  bayers[width *  i    + (j+1)];
				}
				continue;
			}
			else if(j==width-1){ // Right
				if(i%2==0){
					r =  bayers[width *  i    + (j-1)];
					g =  bayers[width *  i    +  j   ];
					b = (bayers[width * (i-1) +  j   ] + bayers[width * (i+1) +  j   ]) / 2;
				}
				else{
					r = (bayers[width * (i-1) + (j-1)] + bayers[width * (i+1) + (j-1)]) / 2;
					g = (bayers[width * (i-1) +  j   ] + bayers[width * (i+1) +  j   ]) / 2;
					b =  bayers[width *  i    +  j   ];
				}
				continue;
			}

			if(i%2==0){
				if(j%2==0){
					r =  bayers[width *  i    +  j   ];
					g = (bayers[width * (i-1) +  j   ] + bayers[width * (i+1) +  j   ] + bayers[width *  i    + (j-1)] + bayers[width *  i    + (j+1)]) / 4;
					b = (bayers[width * (i-1) + (j-1)] + bayers[width * (i-1) + (j+1)] + bayers[width * (i+1) + (j-1)] + bayers[width * (i+1) + (j+1)]) / 4;
				}
				else{
					r = (bayers[width *  i    + (j-1)] + bayers[width *  i    + (j+1)]) / 2;
					g =  bayers[width *  i    +  j   ];
					b = (bayers[width * (i-1) +  j   ] + bayers[width * (i+1) +  j   ]) / 2;
				}
			}
			else{
				if(j%2==0){
					r = (bayers[width * (i-1) +  j   ] + bayers[width * (i+1) +  j   ]) / 2;
					g =  bayers[width *  i    +  j   ];
					b = (bayers[width *  i    + (j-1)] + bayers[width *  i    + (j+1)]) / 2;
				}
				else{
					r = (bayers[width * (i-1) + (j-1)] + bayers[width * (i-1) + (j+1)] + bayers[width * (i+1) + (j-1)] + bayers[width * (i+1) + (j+1)]) / 4;
					g = (bayers[width * (i-1) +  j   ] + bayers[width * (i+1) +  j   ] + bayers[width *  i    + (j-1)] + bayers[width *  i    + (j+1)]) / 4;
					b =  bayers[width *  i    +  j   ];
				}
			}

			if(r<0) r=0; if(g<0) g=0; if(b<0) b=0;
			if(r>1) r=1; if(g>1) g=1; if(b>1) b=1;

			data[3*width*i + 3*j + 0] = r;
			data[3*width*i + 3*j + 1] = g;
			data[3*width*i + 3*j + 2] = b;
		}
	}

	bDemosaiced = true;
	delete[] bayers;
}


void Demosaicer::linearDemosaicGBRG() {

	float* bayers = new float[width*height];
	memcpy(bayers, data, width*height*sizeof(float));

	free(data);
	data = nullptr;
	data = new float[width*height*3];

	for(int i=0; i<height; ++i){
		for(int j=0; j<width; ++j){
			float r=0, g=0, b=0;

			if(i==0 && j==0){ // Left Top
				r =  bayers[width * (i+1) +  j   ];
				g =  bayers[width *  i    +  j   ];
				b =  bayers[width *  i    + (j+1)];
				continue;
			}
			if(i==0 && j==width-1){ // Right Top
				r =  bayers[width * (i+1) + (j-1)];
				g = (bayers[width *  i    + (j-1)] + bayers[width * (i+1) +  j   ]) / 2;
				b =  bayers[width *  i    +  j   ];
				continue;
			}
			if(i==height-1 && j==0){ // Left Bottom
				r =  bayers[width *  i    +  j   ];
				g = (bayers[width * (i-1) +  j   ] + bayers[width *  i    + (j+1)]) / 2;
				b =  bayers[width * (i-1) + (j+1)];
				continue;
			}
			if (i==height-1 && j==width-1) { // Right Bottom
				r =  bayers[width *  i    + (j-1)];
				g =  bayers[width *  i    +  j   ];
				b =  bayers[width * (i-1) +  j   ];
				continue;
			}

			if(i==0){ // Top
				if(j%2==0){
					r =  bayers[width * (i+1) +  j   ];
					g =  bayers[width *  i    +  j   ];
					b = (bayers[width *  i    + (j-1)] + bayers[width *  i    + (j+1)]) / 2;
				}
				else{
					r = (bayers[width * (i+1) + (j-1)] + bayers[width * (i+1) + (j+1)]) / 2;
					g = (bayers[width *  i    + (j-1)] + bayers[width *  i    + (j+1)] + bayers[width * (i+1) + j]) / 3;
					b =  bayers[width *  i    +  j   ];
				}
				continue;
			}
			else if(i==height-1){ // Bottom
				if(j%2==0){
					r =  bayers[width *  i    +  j   ];
					g = (bayers[width *  i    + (j-1)] + bayers[width * (i-1) +  j   ] + bayers[width *  i    + (j+1)]) / 3;
					b = (bayers[width * (i-1) + (j-1)] + bayers[width * (i-1) + (j+1)]) / 2;
				}
				else{
					r = (bayers[width *  i    + (j-1)] + bayers[width *  i    + (j+1)]) / 2;
					g =  bayers[width *  i    +  j   ];
					b =  bayers[width * (i-1) +  j   ];
				}
				continue;
			}
			
			if(j==0){ // Left
				if(i%2==0){
					r = (bayers[width * (i+1) +  j   ] + bayers[width * (i-1) +  j   ]) / 2;
					g =  bayers[width *  i    +  j   ];
					b =  bayers[width *  i    + (j+1)];
				}
				else{
					r =  bayers[width *  i    +  j   ];
					g = (bayers[width * (i+1) +  j   ] + bayers[width * (i-1) +  j   ]) / 2;
					b = (bayers[width * (i+1) + (j+1)] + bayers[width * (i-1) + (j+1)]) / 2;
				}
				continue;
			}
			else if(j==width-1){ // Right
				if(i%2==0){
					r = (bayers[width * (i-1) + (j-1)] + bayers[width * (i+1) + (j-1)]) / 2;
					g = (bayers[width * (i-1) +  j   ] + bayers[width *  i    + (j-1)] + bayers[width * (i+1) +  j   ]) / 3;
					b =  bayers[width *  i    +  j   ];
				}
				else{
					r =  bayers[width *  i    + (j-1)];
					g =  bayers[width *  i    +  j   ];
					b = (bayers[width * (i-1) +  j   ] + bayers[width * (i+1) +  j   ]) / 2;
				}
				continue;
			}

			if(i%2==0){
				if(j%2==0){
					r = (bayers[width * (i+1) +  j   ] + bayers[width * (i-1) +  j   ]) / 2;
					g =  bayers[width *  i    +  j   ];
					b = (bayers[width *  i    + (j-1)] + bayers[width *  i    + (j+1)]) / 2;
				}
				else{
					r = (bayers[width * (i-1) + (j-1)] + bayers[width * (i-1) + (j+1)] + bayers[width * (i+1) + (j-1)] + bayers[width * (i+1) + (j+1)]) / 4;
					g = (bayers[width * (i+1) +  j   ] + bayers[width *  i    + (j+1)] + bayers[width * (i-1) +  j   ] + bayers[width *  i    + (j-1)]) / 4;
					b =  bayers[width *  i    +  j   ];
				}
			}
			else{
				if(j%2==0){
					r =  bayers[width *  i    +  j   ];
					g = (bayers[width * (i+1) +  j   ] + bayers[width *  i    + (j+1)] + bayers[width * (i-1) +  j   ] + bayers[width *  i    + (j-1)]) / 4;
					b = (bayers[width * (i-1) + (j-1)] + bayers[width * (i-1) + (j+1)] + bayers[width * (i+1) + (j-1)] + bayers[width * (i+1) + (j+1)]) / 4;
				}
				else{
					r = (bayers[width *  i    + (j-1)] + bayers[width *  i    + (j+1)]) / 2;
					g =  bayers[width *  i    +  j   ];
					b = (bayers[width * (i+1) +  j   ] + bayers[width * (i-1) +  j   ]) / 2;
				}
			}

			if(r<0) r=0; if(g<0) g=0; if(b<0) b=0;
			if(r>1) r=1; if(g>1) g=1; if(b>1) b=1;

			data[3*width*i + 3*j + 0] = r;
			data[3*width*i + 3*j + 1] = g;
			data[3*width*i + 3*j + 2] = b;
		}
	}

	bDemosaiced = true;
	delete[] bayers;
}
