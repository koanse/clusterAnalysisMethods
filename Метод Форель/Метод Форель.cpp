#include <stdio.h>
#include <conio.h>
#include <math.h>
int n = 15;
int clusters = 0;

struct point
{
	float x;
	float y;
	int cluster;
} *points;

float x[15] = {88, 84, 92, 86, 96, 96, 10, 26, 35, 18, 14, 22, 71, 10, 13 };
float y[15] = {35, 38, 39, 36, 26, 20, 17, 18, 13, 95, 93, 81, 77, 75, 78 };
float R = 0.25f;					// Радиус
float xc, yc;						// Координаты центра сферы
float d(int a);						// Расстояние от центра сферы до точки
void fill_cluster(int a);			// Заполнение кластера

int main()
{
	int i;
	float minx, maxx, miny, maxy;
	bool exist_free_points = true;
	points = new struct point[n];

	// Нормировка
	minx = maxx = x[0];
	miny = maxy = y[0];

	for(i = 1; i < n; i++)
	{
		if(x[i] < minx) minx = x[i];
		if(x[i] > maxx) maxx = x[i];
		if(y[i] < miny) miny = y[i];
		if(y[i] > maxy) maxy = y[i];
	}
		
	for(i = 0; i < n; i++)
	{
		points[i].x = (x[i] - minx) / (maxx - minx);
		points[i].y = (y[i] - miny) / (maxy - miny);
		points[i].cluster = -1;
	}

	// Объединение точек в кластеры
	while(exist_free_points)
	{
        printf("\nTrying to create a new cluster\n"); // Сообщение о попытке создания нового кластера
				
		exist_free_points = false;

		for(i = 0; !exist_free_points && i < n; i++)
			if(points[i].cluster == -1)
			{
                exist_free_points = true;
				clusters++;
				xc = points[i].x;
				yc = points[i].y;
				points[i].cluster = clusters;
				fill_cluster(clusters);
			}
	}


	// Вывод результатов
	printf("It is done. Points and clusters:\n");
	
	for(i = 0; i < n; i++)
		printf("i=%d   x=%f  y=%f  cluster=%d\n",
			i + 1, points[i].x, points[i].y, points[i].cluster);

	getch();

	return 0;
}

float d(int a)
{
	float res = 0;
	res += (points[a].x - xc) * (points[a].x - xc);
	res += (points[a].y - yc) * (points[a].y - yc);
    return powf(res, 0.5);
}


void fill_cluster(int a)
{
	int i, p = 1;
	char *status, in_another_cluster[] = "in another cluster",
		already_in_this_cluster[] = "already in this cluster",
		is_free[] = "is free";
	char *add, yes[] = "can be added (d < R)",
		no[] = "can't be added (d > R)";

	float tmp;
	float xctmp = xc, yctmp = yc;
	xc = yc = -1;

	while(!(xctmp == xc && yctmp == yc))
	{
		xc = xctmp;
		yc = yctmp;
		xctmp = yctmp = 0;
				
		printf("Adding points to cluster %d\n", a); // Сообщение о попытке увеличения кластера
		printf("The center of sphere is (%f;%f), radius is %f\n", xc, yc, R); // Центр и радиус сферы
		printf("Point\tDistance\tStatus\n"); // Шапка таблицы

		for(i = 0; i < n; i++)
		{
			tmp = d(i);
			
			if(tmp < R) add = yes; else add = no;
			if(points[i].cluster == -1) status = is_free;
			if(points[i].cluster == a) status = already_in_this_cluster;
			if(points[i].cluster != a && points[i].cluster != -1) status = in_another_cluster;

			printf("%d\t%f\t%s, %s\n", i + 1, tmp, status, add);	//Расстояния от центра сферы до точек

			if(tmp < R && (points[i].cluster == -1 || points[i].cluster == a))
			{
                xctmp += points[i].x;
				yctmp += points[i].y;
				
				if(points[i].cluster == -1)
				{
					points[i].cluster = clusters;
					p++;
				}
			}
		}

		xctmp /= p;
		yctmp /= p;
	}
	return;
}