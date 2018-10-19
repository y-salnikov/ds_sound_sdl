#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "filter.h"

filter_type* filter_init(int order, const double *k)
{
	filter_type* ft=malloc(sizeof(filter_type));
	if(ft!=NULL)
	{
		ft->order=order;
		ft->values=calloc(order,sizeof(double));
		ft->k=calloc(order,sizeof(double));
		for(int i=0;i<order;i++)
		{
			ft->values[i]=0.0;
			ft->k[i]=k[i];
		}
	}
	return ft;
}

double filter(filter_type * ft, double value)
{
	double s=value*ft->k[0];
	for(int i=1;i<ft->order;i++)
	{
		s=s+ft->values[i-1]*ft->k[i];
		ft->values[i]=ft->values[i-1];
	}
	ft->values[0]=s;
	return s;
}

void filter_done(filter_type * ft)
{
	free(ft->k);
	free(ft->values);
	free(ft);
}

