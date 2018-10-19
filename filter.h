typedef struct
{
	int order;
	double *values;
	double *k;
}
filter_type;

filter_type* filter_init(int order, const double *k);
double filter(filter_type * f, double value);
void filter_done(filter_type * f);

