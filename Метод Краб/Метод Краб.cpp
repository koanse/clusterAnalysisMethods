#include <stdio.h>
#include <conio.h>
#include <math.h>
#define a_big_number 999
int n = 15;
int clusters = 4;					// Нужное количество кластеров

struct point
{
	float x;
	float y;
	int cluster;
} *points;

struct edge
{
	int start, end;
} *edges;

//float x[15] = {41.5, 42.3, 42.0, 38.2, 39.3, 41.5, 45.5, 39.5, 42.0, 49.8, 39.2, 41.9, 45.6, 38.1, 44.2 };
//float y[15] = {26.5, 24.5, 24.5, 25.5, 26.0, 29.5, 30.0, 30.5, 30.5, 30.5, 31.0, 27.0, 27.5, 30.5, 30.5 };

// Вариант 3
//float x[15] = {88, 84, 92, 86, 96, 96, 10, 26, 35, 18, 14, 22, 71, 10, 13 };
//float y[15] = {35, 38, 39, 36, 26, 20, 17, 18, 13, 95, 93, 81, 77, 75, 78 };

// Вариант 25
float x[15] = {82, 88, 84, 86, 81, 82, 78, 84, 84, 84, 78, 86, 85, 84, 78 };
float y[15] = {85, 13, 18, 11, 27, 10, 14, 22, 13, 17, 14, 49, 97, 48, 39 };


float d(int a, int b);				// Расстояние между точками

int main()
{
	int i, j, i_min, j_min, j_max, k, l, p = 0;
	float min, max, minx, maxx, miny, maxy, tmp;
	int *history;
	points = new struct point[n];
	edges = new struct edge[n - 1];
	history = new int[n];

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
		
	minx = miny = 0;
	maxx = maxy = 1;

	for(i = 0; i < n; i++)
	{
		points[i].x = (x[i] - minx) / (maxx - minx);
		points[i].y = (y[i] - miny) / (maxy - miny);
		points[i].cluster = 0;
	}

	// Создание первого ребра
	printf("Table of distinations\nPoint1\tPoint2\tDestination\n"); // Сообщение о выводе расстояний
	
	min = a_big_number;
    for(i = 0; i < n - 1; i++)
		for(j = i + 1; j < n; j++)
		{
			tmp = d(i, j);
			
			printf("%d\t%d\t%f\n", i + 1, j + 1, tmp); // Вывод расстояний

			if(tmp < min)
			{
				min = tmp;
				i_min = i;
				j_min = j;
			}
		}

	points[i_min].cluster = points[j_min].cluster = -1;
	edges[p].start = i_min;
	edges[p].end = j_min;
	p++;

    // Создание кратчайшего незамкнутого пути
	while(p < n - 1)
	{
		min = a_big_number;

		for(i = 0; i < n; i++)
		{
			if(points[i].cluster == 0)
				continue;

			for(j = 0; j < n; j++)
			{
				if(points[j].cluster == -1)
					continue;
				
				tmp = d(i, j);
				if(tmp < min)
				{
					min = tmp;
					i_min = i;
					j_min = j;
				}
			}
		}

		points[i_min].cluster = points[j_min].cluster = -1;
		edges[p].start = i_min;
		edges[p].end = j_min;
		p++;
	}


	printf("Added edges\nN\tStart\tEnd\tD\n"); // Вывод ребер
	for(i = 0; i < n - 1; i++)
		printf("%d\t%d\t%d\t%f\n",
			i + 1, edges[i].start + 1, edges[i].end + 1, d(edges[i].start, edges[i].end));

	
	// Удаление (clusters - 1) ребер
	for(i = 0; i < clusters - 1; i++)
	{
		max = -1;
		for(j = 0; j < p; j++)
		{
            tmp = d(edges[j].start, edges[j].end);
			if(tmp > max)
			{
                max = tmp;
				j_max = j;
			}
		}

		p--;
		
		printf("Delete the edge number %d (start = %d, end = %d)\n", j_max + 1,
			edges[j_max].start + 1, edges[j_max].end + 1); // Вывод удаляемых ребер
		
		for(j = j_max; j < p; j++)
		{
			edges[j].start = edges[j + 1].start;
			edges[j].end = edges[j + 1].end;
		}
	}

	// Формирование кластеров
	for(i = 1; i <= clusters; i++)
	{
		for(j = 0; j < n; j++)
			if(points[j].cluster == -1)
			{
				points[j].cluster = i;
				break;
			}

		l = 0;
		history[l] = j;

		while(l != -1)
		{
			for(k = 0; k < p; k++)
			{
				if(edges[k].start == history[l] && points[edges[k].end].cluster == -1)
				{
					points[edges[k].end].cluster = i;
					l++;
					history[l] = edges[k].end;
					break;
				}

				if(edges[k].end == history[l] && points[edges[k].start].cluster == -1)
				{
					points[edges[k].start].cluster = i;
					l++;
					history[l] = edges[k].start;
					break;
				}
			}
			if(k == p) l--;
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

float d(int a, int b)
{
	float res = 0;
	res += (points[a].x - points[b].x) * (points[a].x - points[b].x);
	res += (points[a].y - points[b].y) * (points[a].y - points[b].y);
    return powf(res, 0.5);
}